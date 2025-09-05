//Configuration.cpp
#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::string Configuration::getDefaultConfigPath() {
    // Ищем корневую папку проекта (где .sln файл)
    fs::path currentPath = fs::current_path();

    // Поднимаемся на несколько уровней вверх из папки Debug/Release
    for (int i = 0; i < 3; ++i) {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            if (entry.path().extension() == ".sln") {
                return (currentPath / "config.ini").string();
            }
        }
        // Поднимаемся на уровень выше
        currentPath = currentPath.parent_path();
    }
    return "config.ini";
}

bool Configuration::loadFromFile(const std::string& configPath) {
    std::ifstream configFile(configPath);
    std::cout << "Загрузка настроек из " << configPath << std::endl;

    if (!configFile.is_open()) {

        fs::path projectConfig = fs::path(configPath).parent_path() / ".." / "config.ini";
        projectConfig = projectConfig.lexically_normal();

        configFile.open(projectConfig);
        if (!configFile.is_open()) {
            throw std::runtime_error("Не удалось открыть файл конфигурации: " + configPath +
                " и " + projectConfig.string());
        }
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(configFile, line)) {
        lineNumber++;

        // Убираем пробелы в начале и конце
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Пропускаем пустые строки, комментарии и секции [section]
        if (line.empty() || line[0] == '#' || line[0] == ';' || line[0] == '[') {
            continue;
        }

        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Предупреждение: некорректная строка " << lineNumber
                << ": " << line << std::endl;
            continue;
        }

        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (!key.empty()) {
            configValues_[key] = value;
        }
    }

    configFile.close();
    return true;
}

bool Configuration::validate() const {
    const std::vector<std::string> requiredKeys = {
        "db_host", "db_port", "db_name", "db_user", "db_password",
        "spider_start_url", "spider_max_depth", "search_engine_port"
    };

    for (const auto& key : requiredKeys) {
        if (configValues_.find(key) == configValues_.end()) {
            std::cerr << "Ошибка: отсутствует обязательный параметр: " << key << std::endl;
            return false;
        }
    }

    // Валидация числовых параметров
    try {
        int depth = getSpiderMaxDepth();
        if (depth < 1 || depth > 5) {
            std::cerr << "Ошибка: глубина рекурсии должна быть от 1 до 5, получено: "
                << depth << std::endl;
            return false;
        }

        int port = getSearchEnginePort();
        if (port < 1024 || port > 65535) {
            std::cerr << "Ошибка: порт должен быть в диапазоне 1024-65535, получено: "
                << port << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка валидации параметров: " << e.what() << std::endl;
        return false;
    }

    return true;
}

std::string Configuration::getValueOrDefault(const std::string& key,
    const std::string& defaultValue) const {
    auto it = configValues_.find(key);
    return it != configValues_.end() ? it->second : defaultValue;
}

// Обязательные параметры (бросают исключение если нет)
std::string Configuration::getDbHost() const { return configValues_.at("db_host"); }
std::string Configuration::getDbPort() const { return configValues_.at("db_port"); }
std::string Configuration::getDbName() const { return configValues_.at("db_name"); }
std::string Configuration::getDbUser() const { return configValues_.at("db_user"); }
std::string Configuration::getDbPassword() const { return configValues_.at("db_password"); }
std::string Configuration::getSpiderStartUrl() const { return configValues_.at("spider_start_url"); }

int Configuration::getSpiderMaxDepth() const {
    try {
        return std::stoi(configValues_.at("spider_max_depth"));
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Неверный формат spider_max_depth: " +
            std::string(e.what()));
    }
}

int Configuration::getSearchEnginePort() const {
    try {
        return std::stoi(configValues_.at("search_engine_port"));
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Неверный формат search_engine_port: " +
            std::string(e.what()));
    }
}