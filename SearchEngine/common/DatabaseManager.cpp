#include "DatabaseManager.h"
#include <pqxx/pqxx>
#include <iostream>
#include <algorithm>

DatabaseManager::DatabaseManager(const std::string& host,
    const std::string& port,
    const std::string& dbname,
    const std::string& user,
    const std::string& password) {

    connection_string_ = "host=" + host + " port=" + port +
        " dbname=" + dbname + " user=" + user +
        " password=" + password;
}

std::vector<std::pair<std::string, double>> DatabaseManager::searchDocuments(const std::vector<std::string>& words) {
    std::vector<std::pair<std::string, double>> results;

    if (words.empty()) {
        throw std::invalid_argument("Список слов для поиска не может быть пустым");
    }

    try {
        auto conn = getConnection();
        if (!conn) {
            throw std::runtime_error("Не удалось подключиться к БД");
        }

        pqxx::work txn(*conn);

        std::string query = R"(
            SELECT d.url, SUM(dw.frequency) as total_relevance
            FROM documents d
            JOIN document_words dw ON d.id = dw.document_id
            JOIN words w ON dw.word_id = w.id
            WHERE w.word = ANY($1)
            GROUP BY d.url
            ORDER BY total_relevance DESC
            LIMIT 10
        )";

        // Создаем строку массива PostgreSQL
        std::string array_str = "{";
        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) array_str += ",";
            array_str += "\"" + txn.esc(words[i]) + "\"";
        }
        array_str += "}";

        auto result = txn.exec(
            "SELECT d.url, SUM(dw.frequency) as total_relevance "
            "FROM documents d "
            "JOIN document_words dw ON d.id = dw.document_id "
            "JOIN words w ON dw.word_id = w.id "
            "WHERE w.word = ANY('" + array_str + "') "
            "GROUP BY d.url "
            "ORDER BY total_relevance DESC "
            "LIMIT 10"
        );

        for (const auto& row : result) {
            std::string url = row[0].as<std::string>();
            double relevance = row[1].as<double>();
            results.emplace_back(url, relevance);
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка поиска документов: " << e.what() << std::endl;
        throw;
    }

    return results;
}

bool DatabaseManager::initialize() {
    try {
        auto conn = getConnection();
        if (!conn) {
            throw std::runtime_error("Не удалось подключиться к базе данных");
        }

        std::cout << "Подключение к БД - успех!" << std::endl;
        pqxx::work txn(*conn);

        // Таблица документов
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS documents (
                id SERIAL PRIMARY KEY,
                url TEXT UNIQUE NOT NULL,
                title TEXT,
                content TEXT,
                description TEXT,
                html_content TEXT,
                last_crawled TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");

        // Таблица слов
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS words (
                id SERIAL PRIMARY KEY,
                word TEXT UNIQUE NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");

        // Промежуточная таблица
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS document_words (
                id SERIAL PRIMARY KEY,
                document_id INTEGER REFERENCES documents(id) ON DELETE CASCADE,
                word_id INTEGER REFERENCES words(id) ON DELETE CASCADE,
                frequency INTEGER NOT NULL DEFAULT 1,
                positions INTEGER[],
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                UNIQUE(document_id, word_id)
            )
        )");

        // Таблица для очереди сканирования
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS crawl_queue (
                id SERIAL PRIMARY KEY,
                url TEXT UNIQUE NOT NULL,
                depth INTEGER NOT NULL DEFAULT 0,
                priority INTEGER DEFAULT 1,
                status TEXT DEFAULT 'pending',
                added_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");

        txn.commit();
        std::cout << "Таблицы базы данных созданы успешно!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка инициализации базы данных: " << e.what();
        return false;
    }
}

void DatabaseManager::createIndexes() {
    try {
        auto conn = getConnection();
        pqxx::work txn(*conn);

        txn.exec(R"(
            CREATE INDEX IF NOT EXISTS idx_words_word ON words(word);
            CREATE INDEX IF NOT EXISTS idx_document_words_word_id ON document_words(word_id);
            CREATE INDEX IF NOT EXISTS idx_document_words_document_id ON document_words(document_id);
            CREATE INDEX IF NOT EXISTS idx_documents_url ON documents(url);
            CREATE INDEX IF NOT EXISTS idx_crawl_queue_status ON crawl_queue(status);
    )");


        txn.commit();
        std::cout << "Индексы для БД созданы успешно!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка создания индексов: " << e.what() << std::endl;
    }
}

void DatabaseManager::cleanupInvalidDocuments() {
    try {
        auto conn = getConnection();
        pqxx::work txn(*conn);

        txn.exec("DELETE FROM documents WHERE content IS NULL OR LENGTH(content) < 100");
        txn.exec("DELETE FROM document_words WHERE document_id NOT IN (SELECT id FROM documents)");
        txn.exec("DELETE FROM words WHERE id NOT IN (SELECT word_id FROM document_words)");

        txn.commit();
        std::cout << "База данных очищена от некорректных записей" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка очистки БД: " << e.what() << std::endl;
    }
}

std::unique_ptr<pqxx::connection> DatabaseManager::getConnection() {
    std::lock_guard<std::mutex> lock(connection_mutex_);

    try {
        return std::make_unique<pqxx::connection>(connection_string_);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Ошибка подключения к БД: " + std::string(e.what()));
    }
}