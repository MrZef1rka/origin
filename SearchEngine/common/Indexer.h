//Indexer.h
#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

class DatabaseManager;

class Indexer {
public:
    Indexer(DatabaseManager& dbManager);

    std::string cleanHtml(const std::string& html) const;
    std::vector<std::string> extractWords(const std::string& text) const;
    std::unordered_map<std::string, int> calculateWordFrequencies(const std::vector<std::string>& words) const;
    bool indexDocument(const std::string& url,
        const std::string& title,
        const std::string& htmlContent,
        const std::string& contentType);

private:
    DatabaseManager& dbManager_;
    mutable std::mutex dbMutex_;
};