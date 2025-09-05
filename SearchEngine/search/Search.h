#pragma once
#include "../common/Configuration.h"
#include "../common/DatabaseManager.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")

class SearchEngine {
public:
    SearchEngine(const Configuration& config, DatabaseManager& dbManager);
    ~SearchEngine();

    void start();
    void stop();

private:
    std::string htmlEscape(const std::string& input);
    void runServer();
    void handleClient(SOCKET clientSocket);
    std::string readRequest(SOCKET clientSocket);
    void sendResponse(SOCKET clientSocket, const std::string& response);

    std::string handleHTTPRequest(const std::string& method, const std::string& path, const std::string& body);
    std::string generateSearchForm();
    std::string generateSearchResults(const std::vector<std::string>& words);
    std::string generate404();
    std::string generateError(const std::string& message);
    std::vector<std::string> parseSearchQuery(const std::string& query);
    std::string urlDecode(const std::string& str);

    const Configuration& config_;
    DatabaseManager& dbManager_;
    std::thread serverThread_;
    std::atomic<bool> stopFlag_{ false };
    SOCKET serverSocket_{ INVALID_SOCKET };
};