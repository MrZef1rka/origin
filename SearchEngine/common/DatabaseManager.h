#pragma once
#include <vector>
#include <string>
#include <memory>
#include <mutex>

namespace pqxx {
    class connection;
}

class DatabaseManager {
public:
    DatabaseManager(const std::string& host,
        const std::string& port,
        const std::string& dbname,
        const std::string& user,
        const std::string& password);

    std::vector<std::pair<std::string, double>> searchDocuments(const std::vector<std::string>& words);
    bool initialize();
    void createIndexes();
    void cleanupInvalidDocuments();
    std::unique_ptr<pqxx::connection> getConnection();

private:
    std::string connection_string_;
    std::mutex connection_mutex_;
};