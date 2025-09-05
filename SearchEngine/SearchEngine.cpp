#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <limits>
#include <shellapi.h>
#include <csignal>
#include <atomic>
#include "common/Configuration.h"
#include "common/DatabaseManager.h"
#include "spider/Spider.h"
#include "search/Search.h"

std::atomic<bool> shutdownRequested{ false };

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        std::cout << "\nReceived shutdown signal..." << std::endl;
        shutdownRequested = true;
        return TRUE;
    }
    return FALSE;
}

void openBrowser(int port) {
    std::string url = "http://localhost:" + std::to_string(port) + "/";
    ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

static void waitForEnter() {
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();
}

int main() {
    setlocale(LC_ALL, "ru_RU");
    std::cout << "=== Search Engine ===" << std::endl;

    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        std::cerr << "Предупреждение: Не удалось установить обработчик" << std::endl;
    }

    try {
        Configuration config;
        config.loadFromFile(config.getDefaultConfigPath());

        if (!config.validate()) {
            std::cerr << "Неверная конфигурация!" << std::endl;
            waitForEnter();
            return 1;
        }

        DatabaseManager dbManager(
            config.getDbHost(),
            config.getDbPort(),
            config.getDbName(),
            config.getDbUser(),
            config.getDbPassword()
        );

        if (!dbManager.initialize()) {
            std::cerr << "Ошибка при создании таблиц!" << std::endl;
            waitForEnter();
            return 1;
        }

        // Запускаем паука
        Spider spider(config);
        spider.start();
        spider.waitForCompletion();
        dbManager.cleanupInvalidDocuments();
        dbManager.createIndexes();

        // Запускаем поисковый сервер
        SearchEngine searchEngine(config, dbManager);
        searchEngine.start();
 
        // Основной цикл с проверкой shutdown
        std::string command;
        std::cout << "Введите 'get' для открытия поисковика или 'exit'(Ctrl+C) для выхода: " << std::endl;
        while (!shutdownRequested) {

            if (!std::getline(std::cin, command)) {
                break;
            }

            if (command == "get" || command == "Get") {
                openBrowser(config.getSearchEnginePort());
            }
            else if (command == "exit" || command == "Exit") {
                break;
            }
            else if (!command.empty()) {
                std::cout << "Неизвестная команда. Введите 'get' или 'exit'(Ctrl+C)!" << std::endl;
            }
        }
        searchEngine.stop();;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        waitForEnter();
        return 1;
    }
    waitForEnter();
    return 0;
}