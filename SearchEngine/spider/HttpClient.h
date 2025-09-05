//HttpClient.h
#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <winhttp.h>
#include <system_error>
#include <mutex>

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    std::string downloadPage(const std::string& url, std::string& outContentType);
    std::vector<std::string> extractLinks(const std::string& html, const std::string& baseUrl);
    bool isValidUrl(const std::string& url);

private:
    std::string getDomainFromUrl(const std::string& url);
    std::string getPathFromUrl(const std::string& url);
    std::string getContentType(HINTERNET hRequest);
    std::string getLastErrorString() const;

    std::mutex winHttpMutex_;
};