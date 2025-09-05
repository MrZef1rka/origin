//Indexer.cpp
#include "Indexer.h"
#include "../common/DatabaseManager.h"
#include <pqxx/pqxx>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <boost/algorithm/string.hpp>

Indexer::Indexer(DatabaseManager& dbManager) : dbManager_(dbManager) {}

std::string Indexer::cleanHtml(const std::string& html) const {
    std::string result;
    bool inTag = false;
    bool inScript = false;
    bool inStyle = false;
    bool inComment = false;

    for (size_t i = 0; i < html.size(); i++) {
        char c = html[i];
        // Пропускаем комментарии
        if (!inComment && i + 3 < html.size() && html.substr(i, 4) == "<!--") {
            inComment = true;
            i += 3;
            continue;
        }
        if (inComment && i + 2 < html.size() && html.substr(i, 3) == "-->") {
            inComment = false;
            i += 2;
            continue;
        }
        if (inComment) continue;

        // Обрабатываем script и style
        if (!inScript && !inStyle) {
            if (i + 6 < html.size() && html.compare(i, 7, "<script") == 0) {
                inScript = true;
                inTag = true;
                i += 6;
                continue;
            }
            if (i + 5 < html.size() && html.compare(i, 6, "<style") == 0) {
                inStyle = true;
                inTag = true;
                i += 5;
                continue;
            }
        }

        if (inScript && i + 8 < html.size() && html.compare(i, 9, "</script>") == 0) {
            inScript = false;
            inTag = false;
            i += 8;
            continue;
        }
        if (inStyle && i + 7 < html.size() && html.compare(i, 8, "</style>") == 0) {
            inStyle = false;
            inTag = false;
            i += 7;
            continue;
        }

        if (c == '<' && !inScript && !inStyle) {
            inTag = true;
            continue;
        }
        if (c == '>' && !inScript && !inStyle) {
            inTag = false;
            continue;
        }

        if (!inTag && !inScript && !inStyle) {
            // Заменяем HTML entities
            if (c == '&') {
                size_t semicolon = html.find(';', i);
                if (semicolon != std::string::npos && semicolon - i < 10) {
                    std::string entity = html.substr(i, semicolon - i + 1);
                    if (entity == "&amp;") result += '&';
                    else if (entity == "&lt;") result += '<';
                    else if (entity == "&gt;") result += '>';
                    else if (entity == "&quot;") result += '"';
                    else if (entity == "&apos;") result += '\'';
                    else if (entity == "&nbsp;") result += ' ';
                    else if (entity == "&copy;") result += "(c)";
                    else result += ' ';
                    i = semicolon;
                }
                else {
                    result += ' ';
                }
            }
            // Сохраняем только буквы, цифры и пробелы
            if (std::isalpha(c)) {
                result += std::tolower(c);
            }
            else if (c == ' ') {
                result += ' ';
            }
            else {
                result += ' ';
            }
        }
    }

    // Убираем лишние пробелы и нормализуем
    std::string cleaned;
    cleaned.reserve(result.size());
    bool lastWasSpace = false;

    for (char c : result) {
        if (std::isspace(c)) {
            if (!lastWasSpace) {
                cleaned += ' ';
                lastWasSpace = true;
            }
        }
        else {
            cleaned += c;
            lastWasSpace = false;
        }
    }

    // Обрезаем пробелы по краям
    if (!cleaned.empty() && std::isspace(cleaned[0])) {
        cleaned.erase(0, 1);
    }
    if (!cleaned.empty() && std::isspace(cleaned.back())) {
        cleaned.pop_back();
    }

    return cleaned;
}

std::vector<std::string> Indexer::extractWords(const std::string& text) const {
    std::vector<std::string> words;
    std::istringstream iss(text);
    std::string word;

    static const std::unordered_set<std::string> stopWords = {
        "the", "and", "for", "with", "that", "this", "are", "from",
        "you", "your", "they", "their", "which", "would", "could",
        "should", "will", "have", "has", "had", "been", "what",
        "when", "where", "why", "how", "who", "whom", "whose",
        "about", "into", "over", "under", "after", "before", "between"
    };


    while (iss >> word) {
        // Удаляем все не-буквенные символы
        word.erase(std::remove_if(word.begin(), word.end(),
            [](char c) { return !std::isalpha(c); }), word.end());

        // Проверяем что остались только буквы
        if (!word.empty() &&
            word.size() >= 3 &&
            word.size() <= 32 &&
            stopWords.find(word) == stopWords.end() &&
            std::all_of(word.begin(), word.end(), ::isalpha)) {
            words.push_back(word);
        }
    }

    return words;
}

std::unordered_map<std::string, int> Indexer::calculateWordFrequencies(
    const std::vector<std::string>& words) const {

    std::unordered_map<std::string, int> frequencies;
    for (const auto& word : words) {
        frequencies[word]++;
    }
    return frequencies;
}

bool Indexer::indexDocument(const std::string& url,
    const std::string& title,
    const std::string& htmlContent,
    const std::string& contentType) {

    std::lock_guard<std::mutex> lock(dbMutex_);

    try {
        auto conn = dbManager_.getConnection();
        if (!conn) return false;

        std::string cleanText = cleanHtml(htmlContent);
        /*std::cout << "Текст после очистки: " << cleanText.substr(0, 200) << "..." << std::endl;*/

        std::vector<std::string> words = extractWords(cleanText);

        auto frequencies = calculateWordFrequencies(words);

        pqxx::work txn(*conn);

        // Вставляем документ
        txn.exec(
            "INSERT INTO documents (url, title, content, html_content) "
            "VALUES (" + txn.quote(url) + ", " + txn.quote(title) + ", " +
            txn.quote(cleanText) + ", " + txn.quote(htmlContent) + ") "
            "ON CONFLICT (url) DO UPDATE SET "
            "title = EXCLUDED.title, content = EXCLUDED.content, "
            "html_content = EXCLUDED.html_content, last_crawled = CURRENT_TIMESTAMP"
        );

        // Получаем ID документа
        auto docResult = txn.exec("SELECT id FROM documents WHERE url = " + txn.quote(url));
        int documentId = docResult[0][0].as<int>();

        // Обрабатываем слова
        for (const auto& [word, frequency] : frequencies) {
            // Вставляем слово
            txn.exec(
                "INSERT INTO words (word) VALUES (" + txn.quote(word) + ") "
                "ON CONFLICT (word) DO NOTHING"
            );

            // Получаем ID слова
            auto wordResult = txn.exec("SELECT id FROM words WHERE word = " + txn.quote(word));
            int wordId = wordResult[0][0].as<int>();

            // Вставляем связь
            txn.exec(
                "INSERT INTO document_words (document_id, word_id, frequency) "
                "VALUES (" + std::to_string(documentId) + ", " + std::to_string(wordId) +
                ", " + std::to_string(frequency) + ") "
                "ON CONFLICT (document_id, word_id) DO UPDATE SET "
                "frequency = EXCLUDED.frequency"
            );
        }

        txn.commit();
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка индексации " << url << ": " << e.what() << std::endl;
        return false;
    }
}