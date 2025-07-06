#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class TCPServer {
private:
    SOCKET serverSocket;
    std::vector<std::thread> clientThreads;
    bool isRunning;

    void HandleClient(SOCKET clientSocket) {
        char buffer[1024];
        int bytesReceived;

        // Получаем данные от клиента
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Ошибка при получении данных: " << WSAGetLastError() << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "Клиент отключился" << std::endl;
        } else {
            buffer[bytesReceived] = '\0';
            std::cout << "Получено от клиента: " << buffer << std::endl;

            // Отправляем ответ
            const char* response = "Сообщение получено сервером";
            send(clientSocket, response, strlen(response), 0);
        }

        // Закрываем соединение
        closesocket(clientSocket);
    }

public:
    TCPServer() : serverSocket(INVALID_SOCKET), isRunning(false) {}

    ~TCPServer() {
        Stop();
    }

    bool Start(int port) {
        // Инициализация Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Ошибка инициализации Winsock" << std::endl;
            return false;
        }

        // Создание сокета
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }

        // Настройка адреса сервера
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        // Привязка сокета
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Ошибка привязки сокета: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        // Прослушивание соединений
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Ошибка прослушивания: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        std::cout << "Сервер запущен на порту " << port << std::endl;
        isRunning = true;

        // Основной цикл сервера
        while (isRunning) {
            // Принятие нового соединения
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
            
            if (clientSocket == INVALID_SOCKET) {
                if (isRunning) {
                    std::cerr << "Ошибка принятия соединения: " << WSAGetLastError() << std::endl;
                }
                continue;
            }

            // Вывод информации о клиенте
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::cout << "Новое соединение от " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

            // Создание потока для обработки клиента
            clientThreads.emplace_back(&TCPServer::HandleClient, this, clientSocket);
        }

        return true;
    }

    void Stop() {
        isRunning = false;
        
        // Закрытие сокета для выхода из accept()
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }

        // Ожидание завершения всех клиентских потоков
        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        clientThreads.clear();

        // Очистка Winsock
        WSACleanup();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <порт>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    TCPServer server;

    if (!server.Start(port)) {
        return 1;
    }

    // Ожидание команды на завершение
    std::cout << "Сервер работает. Нажмите Enter для остановки..." << std::endl;
    std::cin.get();

    server.Stop();
    return 0;
}
