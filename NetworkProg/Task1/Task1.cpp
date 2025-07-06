#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class TCPClient {
private:
    WSADATA wsaData;
    SOCKET clientSocket = INVALID_SOCKET;
    sockaddr_in serverAddress{};

public:
    TCPClient() {
        // Инициализация Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
    }

    ~TCPClient() {
        disconnect();
        WSACleanup();
    }

    void connect(const std::string& ip, int port) {
        // Создание сокета
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
        }

        // Настройка адреса сервера
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address or address not supported");
        }

        // Подключение к серверу
        if (::connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            throw std::runtime_error("Connection failed: " + std::to_string(WSAGetLastError()));
        }

        std::cout << "Connected to server " << ip << ":" << port << std::endl;
    }

    void disconnect() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
            std::cout << "Disconnected from server" << std::endl;
        }
    }

    void sendData(const std::string& message) {
        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
        }
        std::cout << "Sent: " << message << std::endl;
    }

    std::string receiveData(int bufferSize = 1024) {
        char* buffer = new char[bufferSize];
        int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
        
        if (bytesReceived == SOCKET_ERROR) {
            delete[] buffer;
            throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
        }

        std::string receivedData(buffer, bytesReceived);
        delete[] buffer;
        
        std::cout << "Received (" << bytesReceived << " bytes): " << receivedData << std::endl;
        return receivedData;
    }
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
            return 1;
        }

        TCPClient client;
        client.connect(argv[1], std::stoi(argv[2]));
        
        // Отправка тестового сообщения
        client.sendData("Hello, TCP Server!");
        
        // Получение ответа
        auto response = client.receiveData();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}