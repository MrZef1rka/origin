//HttpClient.cpp
#include "HttpClient.h"
#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include <boost/algorithm/string.hpp>

#pragma comment(lib, "winhttp.lib")

HttpClient::HttpClient() {}
HttpClient::~HttpClient() {}

std::string HttpClient::getLastErrorString() const {
    DWORD error = GetLastError();
    if (error == 0) return "No error";

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
}

std::string HttpClient::downloadPage(const std::string& url, std::string& outContentType) {

    std::lock_guard<std::mutex> lock(winHttpMutex_);
    //std::cout << "Загрузка: " << url << std::endl;

    HINTERNET hSession = nullptr, hConnect = nullptr, hRequest = nullptr;
    outContentType.clear();

    auto cleanup = [&]() {
        if (hRequest) WinHttpCloseHandle(hRequest);
        if (hConnect) WinHttpCloseHandle(hConnect);
        if (hSession) WinHttpCloseHandle(hSession);
        };

    try {
        // 1. Инициализация сессии
        hSession = WinHttpOpen(L"SearchEngine/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);

        if (!hSession) {
            if (!hSession) throw std::runtime_error("WinHttpOpen failed: " + getLastErrorString());
        }

        // 2. Парсим URL
        std::string domain = getDomainFromUrl(url);
        std::string path = getPathFromUrl(url);

        std::wstring wideDomain(domain.begin(), domain.end());
        std::wstring widePath(path.begin(), path.end());

        // 3. Подключаемся к серверу
        hConnect = WinHttpConnect(hSession, wideDomain.c_str(),
            INTERNET_DEFAULT_HTTPS_PORT, 0);

        if (!hConnect) {
            std::cerr << "WinHttpConnect failed: " << getLastErrorString() << std::endl;
            throw std::runtime_error("WinHttpConnect failed");
        }

        // 4. Создаем запрос
        hRequest = WinHttpOpenRequest(hConnect, L"GET", widePath.c_str(),
            nullptr, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE); // Флаг для HTTPS

        if (!hRequest) {
            std::cerr << "WinHttpOpenRequest failed: " << getLastErrorString() << std::endl;
            throw std::runtime_error("WinHttpOpenRequest failed");
        }

        // 5. Настраиваем редиректы
        DWORD redirectPolicy = WINHTTP_OPTION_REDIRECT_POLICY_ALWAYS;
        if (!WinHttpSetOption(hRequest, WINHTTP_OPTION_REDIRECT_POLICY, &redirectPolicy, sizeof(redirectPolicy))) {
            std::cerr << "WinHttpSetOption failed: " << getLastErrorString() << std::endl;
        }

        // 6. Таймауты
        DWORD timeout = 30000;
        WinHttpSetTimeouts(hRequest, timeout, timeout, timeout, timeout);

        // 7. Отправляем запрос
        if (!WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
            std::cerr << "WinHttpSendRequest failed: " << getLastErrorString() << std::endl;
            throw std::runtime_error("WinHttpSendRequest failed");
        }

        // 8. Получаем ответ
        if (!WinHttpReceiveResponse(hRequest, nullptr)) {
            std::cerr << "WinHttpReceiveResponse failed: " << getLastErrorString() << std::endl;
            throw std::runtime_error("WinHttpReceiveResponse failed");
        }

        // 9. Получаем Content-Type
        outContentType = getContentType(hRequest);

        // 10. Читаем данные
        std::string response;
        DWORD bytesRead = 0;
        char buffer[8192];

        do {
            if (!WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead)) {
                throw std::runtime_error("WinHttpReadData failed: " + getLastErrorString());
            }

            if (bytesRead > 0) {
                response.append(buffer, bytesRead);
            }
        } while (bytesRead > 0);

        // 11. Закрываем handles
        cleanup();

        return response;

    }
    catch (const std::exception& e) {
        cleanup();
        std::cerr << "Ошибка загрузки " << url << ": " << e.what() << std::endl;
        throw;        
    }
}

std::string HttpClient::getDomainFromUrl(const std::string& url) {
    size_t start = url.find("://");
    if (start == std::string::npos) return "";
    start += 3;

    size_t end = url.find("/", start);
    if (end == std::string::npos) return url.substr(start);

    return url.substr(start, end - start);
}

std::string HttpClient::getPathFromUrl(const std::string& url) {
    size_t start = url.find("://");
    if (start == std::string::npos) return "/";
    start += 3;

    size_t pathStart = url.find("/", start);
    if (pathStart == std::string::npos) return "/";

    return url.substr(pathStart);
}

std::string HttpClient::getContentType(HINTERNET hRequest) {
    DWORD dwSize = 0;
    if (!WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_TYPE,
        WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSize, NULL) &&
        GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

        std::vector<wchar_t> buffer(dwSize / sizeof(wchar_t));
        if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_TYPE,
            WINHTTP_HEADER_NAME_BY_INDEX, &buffer[0], &dwSize, NULL)) {

            int size_needed = WideCharToMultiByte(CP_UTF8, 0, &buffer[0], -1, NULL, 0, NULL, NULL);
            std::string str(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, &buffer[0], -1, &str[0], size_needed, NULL, NULL);
            return str;
        }
    }
    return "unknown";
}

std::vector<std::string> HttpClient::extractLinks(const std::string& html, const std::string& baseUrl) {
    std::vector<std::string> links;
    size_t pos = 0;

    while ((pos = html.find("href=\"", pos)) != std::string::npos) {
        pos += 6;
        size_t endPos = html.find("\"", pos);
        if (endPos == std::string::npos) break;

        std::string link = html.substr(pos, endPos - pos);

        // Пропускаем невалидные ссылки
        if (link.empty() || link[0] == '#' ||
            link.find("javascript:") == 0 || link.find("mailto:") == 0) {
            pos = endPos + 1;
            continue;
        }

        // Преобразуем относительные ссылки
        if (link.find("http") != 0) {
            if (link[0] == '/') {
                link = "https://" + getDomainFromUrl(baseUrl) + link;
            }
            else {
                // Пропускаем сложные относительные ссылки для простоты
                pos = endPos + 1;
                continue;
            }
        }

        // Только HTTPS для безопасности
        if (link.find("https://") == 0 && isValidUrl(link)) {
            links.push_back(link);
        }

        pos = endPos + 1;
    }

    return links;
}

bool HttpClient::isValidUrl(const std::string& url) {
    return url.find("https://") == 0;
}