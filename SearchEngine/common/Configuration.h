//Configuration.h
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class Configuration {
public:
    Configuration() = default;
    std::string getDefaultConfigPath();
    bool loadFromFile(const std::string& configPath);
    bool validate() const;

    // Обязательные параметры
    std::string getDbHost() const;
    std::string getDbPort() const;
    std::string getDbName() const;
    std::string getDbUser() const;
    std::string getDbPassword() const;
    std::string getSpiderStartUrl() const;
    int getSpiderMaxDepth() const;
    int getSearchEnginePort() const;

    // Опциональные параметры с значениями по умолчанию
    std::string getValueOrDefault(const std::string& key,
        const std::string& defaultValue = "") const;

private:
    std::unordered_map<std::string, std::string> configValues_;
};