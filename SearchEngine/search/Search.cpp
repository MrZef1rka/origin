#include "Search.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

SearchEngine::SearchEngine(const Configuration& config, DatabaseManager& dbManager)
    : config_(config), dbManager_(dbManager) {
}

SearchEngine::~SearchEngine() {
    stop();
}

void SearchEngine::start() {
    std::cout << "Запуск поискового сервера.." << std::endl;
    if (stopFlag_) return;

    serverThread_ = std::thread([this]() { runServer(); });
    std::cout << "Сервер запущен!" << std::endl;
}

void SearchEngine::stop() {
    if (stopFlag_) return;

    stopFlag_ = true;

    // Закрываем серверный сокет
    if (serverSocket_ != INVALID_SOCKET) {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
    }

    if (serverThread_.joinable()) {
        serverThread_.join();
    }

    WSACleanup();
}

// === БЕЗОПАСНОЕ ЭКРАНИРОВАНИЕ HTML ===
std::string SearchEngine::htmlEscape(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 2);

    for (char c : input) {
        switch (c) {
        case '&':  output += "&amp;";  break;
        case '<':  output += "&lt;";   break;
        case '>':  output += "&gt;";   break;
        case '"':  output += "&quot;"; break;
        case '\'': output += "&#39;";  break;
        default:   output += c;        break;
        }
    }
    return output;
}

void SearchEngine::runServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return;
    }

    // Создаем сокет с опцией REUSEADDR
    serverSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket_ == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Устанавливаем опцию повторного использования адреса
    int yes = 1;
    if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) == SOCKET_ERROR) {
        std::cerr << "Setsocket failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket_);
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(config_.getSearchEnginePort());

    if (bind(serverSocket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket_);
        WSACleanup();
        return;
    }

    if (listen(serverSocket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket_);
        WSACleanup();
        return;
    }

    while (!stopFlag_) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket_, &readSet);

        timeval timeout{ 1, 0 }; // 1 секунда

        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        if (result == SOCKET_ERROR) {
            if (!stopFlag_) {
                std::cerr << "Select failed: " << WSAGetLastError() << std::endl;
            }
            break;
        }

        if (result > 0 && FD_ISSET(serverSocket_, &readSet)) {
            sockaddr_in clientAddr{};
            int clientAddrSize = sizeof(clientAddr);

            SOCKET clientSocket = accept(serverSocket_, (sockaddr*)&clientAddr, &clientAddrSize);
            if (clientSocket == INVALID_SOCKET) {
                if (!stopFlag_) {
                    std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                }
                continue;
            }

            // Обрабатываем клиента в отдельном потоке для неблокирующей работы
            std::thread([this, clientSocket]() {
                handleClient(clientSocket);
                closesocket(clientSocket);
                }).detach();
        }
    }

    if (serverSocket_ != INVALID_SOCKET) {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
    }
    WSACleanup();
}

void SearchEngine::handleClient(SOCKET clientSocket) {
    if (stopFlag_) return;

    try {
        std::string request = readRequest(clientSocket);
        if (request.empty() || stopFlag_) return;

        std::istringstream iss(request);
        std::string method, path, protocol;
        iss >> method >> path >> protocol;

        std::string body;
        size_t bodyPos = request.find("\r\n\r\n");
        if (bodyPos != std::string::npos) {
            body = request.substr(bodyPos + 4);
        }

        std::string response = handleHTTPRequest(method, path, body);
        sendResponse(clientSocket, response);

    }
    catch (const std::exception& e) {
        if (!stopFlag_) {
            std::cerr << "Request processing error: " << e.what() << std::endl;
        }
        std::string errorResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nInternal Server Error";
        sendResponse(clientSocket, errorResponse);
    }
}

std::string SearchEngine::readRequest(SOCKET clientSocket) {
    if (stopFlag_) return "";

    std::string request;
    char buffer[4096];
    int bytesRead;

    // Устанавливаем таймаут
    timeval timeout{ 2, 0 };
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    while (!stopFlag_) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            request.append(buffer, bytesRead);
            if (request.find("\r\n\r\n") != std::string::npos) {
                break;
            }
        }
        else if (bytesRead == 0) {
            break; // Connection closed
        }
        else {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                break;
            }
        }
    }

    return request;
}

void SearchEngine::sendResponse(SOCKET clientSocket, const std::string& response) {
    if (stopFlag_) return;

    int totalSent = 0;
    int toSend = static_cast<int>(response.size());
    const char* data = response.c_str();

    while (totalSent < toSend && !stopFlag_) {
        int sent = send(clientSocket, data + totalSent, toSend - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        else {
            totalSent += sent;
        }
    }
}

std::string SearchEngine::handleHTTPRequest(const std::string& method, const std::string& path, const std::string& body) {
    try {
        if (method == "GET") {
            if (path == "/" || path == "/index.html") {
                return generateSearchForm();
            }
            else if (path == "/favicon.ico") {
                return "HTTP/1.1 404 Not Found\r\n\r\n";
            }
        }
        else if (method == "POST" && path == "/search") {
            size_t pos = body.find("query=");
            if (pos != std::string::npos) {
                std::string query = body.substr(pos + 6);
                query = urlDecode(query);

                auto words = parseSearchQuery(query);

                // ВАЖНО: Проверка для обеих форм
                if (words.empty()) {
                    return generateError("Please enter at least one search word");
                }
                if (words.size() > 4) {
                    words.resize(4);
                }

                return generateSearchResults(words);
            }
            else {
                return generateError("Invalid request format");
            }
        }
        return generate404();
    }
    catch (const std::exception& e) {
        return generateError("Internal server error: " + std::string(e.what()));
    }
}

std::string SearchEngine::generateSearchForm() {
    return R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8
Connection: close

<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Search Engine</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            margin: 40px; 
            text-align: center; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            background: white;
            border-radius: 20px;
            padding: 40px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            width: 100%;
            max-width: 500px;
        }
        .search-form { 
            margin: 20px auto; 
            max-width: 400px; 
            display: flex;           
            flex-direction: column;  
            align-items: center; 
        }
        .search-input { 
            padding: 15px 20px; 
            width: 100%;       
            font-size: 16px; 
            border: 2px solid #e1e5e9; 
            border-radius: 50px; 
            outline: none;
            margin-bottom: 15px;
            box-sizing: border-box;
        }
        .search-input:focus {
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }
        .search-button { 
            padding: 15px 30px; 
            font-size: 16px; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white; 
            border: none; 
            border-radius: 50px; 
            cursor: pointer;
            width: 100%;           
            font-weight: 600;
            box-sizing: border-box; 
        }
        .search-button:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 20px rgba(102, 126, 234, 0.3);
        }
        .hint {
            margin-top: 20px;
            color: #666;
            font-size: 14px;
        }
        .error-hint {
            margin-top: 10px;
            color: #ff4444;
            font-size: 12px;
            display: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Search Engine</h1>
        <form class="search-form" action="/search" method="post">
            <input type="text" name="query" class="search-input" 
                   placeholder="Enter search query..." required autocomplete="off"
                   pattern="[a-zA-Z\s]+" 
                   title="Only English letters and spaces allowed">
            <button type="submit" class="search-button">Search</button>
        </form>
        <div class="hint">
            You can search up to 4 words separated by spaces
        </div>
        <div class="error-hint" id="errorHint">
            Only English letters and spaces allowed
        </div>
    </div>
    <script>
        const input = document.querySelector('.search-input');
        const errorHint = document.getElementById('errorHint');
        
        input.addEventListener('input', function() {
            const isValid = /^[a-zA-Z\s]*$/.test(this.value);
            if (!isValid && this.value.length > 0) {
                errorHint.style.display = 'block';
                this.setCustomValidity('Only English letters and spaces allowed');
            } else {
                errorHint.style.display = 'none';
                this.setCustomValidity('');
            }
        });
    </script>
</body>
</html>)";
}

std::string SearchEngine::generateSearchResults(const std::vector<std::string>& words) {
    try {
        auto results = dbManager_.searchDocuments(words);

        std::stringstream html;
        html << R"(HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8
Connection: close

<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Search Results</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            margin: 40px; 
            background: #f5f7fa;
        }
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px 20px;
            text-align: center;
            margin-bottom: 30px;
            border-radius: 10px;
        }
        .search-box {
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
            margin-bottom: 30px;
        }
        .search-form {
            display: flex;
            gap: 10px;
        }
        .search-input {
            flex: 1;
            padding: 12px 15px;
            border: 2px solid #e1e5e9;
            border-radius: 25px;
            font-size: 16px;
            outline: none;
        }
        .search-input:focus {
            border-color: #667eea;
        }
        .search-button {
            background: #667eea;
            color: white;
            border: none;
            padding: 12px 25px;
            border-radius: 25px;
            cursor: pointer;
            font-size: 16px;
        }
        .result { 
            margin-bottom: 20px; 
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        .result a { 
            font-size: 18px; 
            color: #1a0dab; 
            text-decoration: none; 
        }
        .result a:hover { 
            text-decoration: underline; 
        }
        .relevance { 
            color: #006621; 
            font-size: 14px; 
        }
        .back-link {
            display: inline-block;
            margin-top: 20px;
            color: #667eea;
            text-decoration: none;
            font-size: 16px;
        }
        .back-link:hover {
            text-decoration: underline;
        }
        .error-hint {
            margin-top: 10px;
            color: #ff4444;
            font-size: 12px;
            display: none;
        }
        .error-message {
            background: #fff3f3;
            border: 1px solid #ff4444;
            color: #ff4444;
            padding: 15px;
            border-radius: 10px;
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
    <div class="header">
        <h1>Search Results</h1>
    </div>
    
    <div class="search-box">
        <form class="search-form" action="/search" method="post">
            <input type="text" name="query" class="search-input" 
                   value=")";

        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) html << " ";
            html << htmlEscape(words[i]);
        }

        html << R"(" placeholder="Enter new query..." 
                   pattern="[a-zA-Z\s]+" 
                   title="Only English letters and spaces allowed"
                   required>
            <button type="submit" class="search-button">Search</button>
        </form>
        <div class="error-hint" id="errorHint">
            Only English letters and spaces allowed
        </div>
    </div>
    
    <h2>Results for: ")";

        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) html << " ";
            html << htmlEscape(words[i]);
        }

        html << R"("</h2>
    
    <div class="results">)";

        if (results.empty()) {
            html << "<p>No results found for your query</p>";
        }
        else {
            for (const auto& [url, relevance] : results) {
                html << R"(
                <div class="result">
                    <a href=")" << htmlEscape(url) << R"(" target="_blank">)"
                    << htmlEscape(url) << R"(</a><br>
                    <span class="relevance">Relevance: )" << relevance << R"(</span>
                </div>)";
            }
        }

        html << R"(
    </div>
    <br>
    <a href="/" class="back-link">New Search</a>
    
    <script>
        const input = document.querySelector('.search-input');
        const errorHint = document.getElementById('errorHint');
        const form = document.querySelector('.search-form');
        
        input.addEventListener('input', function() {
            const isValid = /^[a-zA-Z\s]*$/.test(this.value);
            if (!isValid && this.value.length > 0) {
                errorHint.style.display = 'block';
                this.setCustomValidity('Only English letters and spaces allowed');
            } else {
                errorHint.style.display = 'none';
                this.setCustomValidity('');
            }
        });
        
        form.addEventListener('submit', function(e) {
            if (input.value.trim().length === 0) {
                e.preventDefault();
                errorHint.style.display = 'block';
                errorHint.textContent = 'Please enter at least one word';
                input.setCustomValidity('Please enter at least one word');
            }
        });
    </script>
</body>
</html>)";

        return html.str();
    }
    catch (const std::exception& e) {
        return generateError("Search error: " + std::string(e.what()));
    }
}

std::string SearchEngine::generate404() {
    return R"(HTTP/1.1 404 Not Found
Content-Type: text/html; charset=utf-8
Connection: close

<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>404 - NOT FOUND </title>
</head>
<body>
    <h1>404</h1>
    <p>NOT FOUND</p>
    <a href="/">Go back to the main page</a>
</body>
</html>)";
}

std::string SearchEngine::generateError(const std::string& message) {
    return R"(HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html; charset=utf-8\r\nConnection: close\r\n\r\n)"
        + std::string("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Error</title></head>")
        + "<body><h1>Error</h1><p>" + htmlEscape(message) + "</p>"
        + "<a href='/'>Return to main page</a></body></html>";
}

std::vector<std::string> SearchEngine::parseSearchQuery(const std::string& query) {
    std::vector<std::string> words;
    std::stringstream ss(query);
    std::string word;

    while (ss >> word) {
        // Удаляем все не-буквенные символы
        word.erase(std::remove_if(word.begin(), word.end(),
            [](unsigned char c) { return !std::isalpha(c); }), word.end());

        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (word.length() >= 1) {
            words.push_back(word);
        }
    }

    return words;
}

std::string SearchEngine::urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '%' && i + 2 < str.size()) {
            int value;
            std::istringstream hexStream(str.substr(i + 1, 2));
            if (hexStream >> std::hex >> value && value > 0 && value < 256) {
                result += static_cast<char>(value);
                i += 2;
            }
            else {
                result += str[i];
            }
        }
        else if (str[i] == '+') {
            result += ' ';
        }
        else {
            result += str[i];
        }
    }
    return result;
}