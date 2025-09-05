//Spider.cpp
#include "Spider.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

Spider::Spider(const Configuration& config)
    : config_(config)
    , dbManager_(config.getDbHost(), config.getDbPort(),
        config.getDbName(), config.getDbUser(), config.getDbPassword())
    , indexer_(dbManager_) {
}

Spider::~Spider() {
    stop();
}

void Spider::start() {
    std::cout << "=== Запуск многопоточного паука ===" << std::endl;
    std::cout << "Стартовая страница: " << config_.getSpiderStartUrl() << std::endl;
    std::cout << "Глубина рекурсии: " << config_.getSpiderMaxDepth() << std::endl;

    // Очищаем очередь и посещенные URL
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        while (!urlQueue_.empty()) urlQueue_.pop();
        visitedUrls_.clear();
        stopFlag_ = false;
        activeTasks_ = 0;
        allTasksCompleted_ = false;
        processedCount_ = 0;
    }

    // Добавляем стартовый URL с глубиной 0
    addUrlToQueue(config_.getSpiderStartUrl(), 0);

    // Создаем пул рабочих потоков
    unsigned int numThreads = min(8u, std::thread::hardware_concurrency());
    if (numThreads == 0) numThreads = 2;

    std::cout << "Запуск " << numThreads << " рабочих потоков..." << std::endl;

    // Запускаем индикатор прогресса
    stopProgress_ = false;
    progressThread_ = std::thread([this]() {
        const char spinner[] = { '|', '/', '-', '\\' };
        int spinnerIndex = 0;

        while (!stopProgress_ && !stopFlag_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            std::lock_guard<std::mutex> lock(queueMutex_);

            if (urlQueue_.empty() && activeTasks_ == 0) {
                break;
            }

            std::cout << "\rИндексация... " << spinner[spinnerIndex]
                << " Обработано: " << processedCount_
                << " | В очереди: " << urlQueue_.size()
                << " | Активных: " << activeTasks_
                << "          ";
            std::cout.flush();

            spinnerIndex = (spinnerIndex + 1) % 4;
        }
        std::cout << "\r" << std::string(100, ' ') << "\r";
        std::cout << "Индексация завершена!";
        });

    try {
        // Запускаем рабочие потоки
        for (unsigned int i = 0; i < numThreads; ++i) {
            workers_.emplace_back(&Spider::workerThread, this);
        }

        // Ждем завершения рабочих потоков
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    catch (...) {
        // При любой ошибке останавливаем progress thread
        stopProgress_ = true;
        if (progressThread_.joinable()) {
            progressThread_.join();
        }
        throw;
    }
}

void Spider::stop() {
    if (stopFlag_) return;
    stopFlag_ = true;
    stopProgress_ = true;
    queueCV_.notify_all();

    // Ждем завершения progress thread
    if (progressThread_.joinable()) {
        progressThread_.join();
    }

    // Ждем завершения рабочих потоков
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();
}

void Spider::waitForCompletion() {
    // Ждем завершения всех потоков
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();

    // Останавливаем progress thread
    stopProgress_ = true;
    if (progressThread_.joinable()) {
        progressThread_.join();
    }

    std::cout << "\n=== Паук завершил работу ===" << std::endl;
    std::cout << "Обработано страниц: " << processedCount_ << std::endl;
}

void Spider::workerThread() {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        activeWorkers_++;
    }

    try {
        while (!stopFlag_ && !allTasksCompleted_) {
            std::pair<std::string, int> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex_);

                // Ждем пока появится задача или завершение работы
                queueCV_.wait(lock, [this]() {
                    return !urlQueue_.empty() || stopFlag_ || allTasksCompleted_;
                    });

                if (stopFlag_ || allTasksCompleted_) {
                    break;
                }

                if (urlQueue_.empty()) {
                    continue;
                }

                task = urlQueue_.front();
                urlQueue_.pop();
                activeTasks_++;
            }

            processUrl(task.first, task.second);

            {
                std::lock_guard<std::mutex> lock(queueMutex_);
                activeTasks_--;

                // Проверяем завершение всех задач
                if (urlQueue_.empty() && activeTasks_ == 0) {
                    allTasksCompleted_ = true;
                    queueCV_.notify_all();
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка в рабочем потоке: " << e.what() << std::endl;
        std::lock_guard<std::mutex> lock(queueMutex_);
        activeTasks_--;
        stop();
    }
    catch (...) {
        std::cerr << "Неизвестная ошибка в рабочем потоке" << std::endl;
        std::lock_guard<std::mutex> lock(queueMutex_);
        activeTasks_--;
        stop();
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        activeWorkers_--;
    }
}

void Spider::addUrlToQueue(const std::string& url, int depth) {
    std::unique_lock<std::mutex> lock(queueMutex_);

    // Ограничение размера очереди (100k URL)
    if (urlQueue_.size() > 100000) {
        return;
    }

    if (visitedUrls_.find(url) == visitedUrls_.end()) {
        urlQueue_.push({ url, depth });
        visitedUrls_.insert(url);

        // Сбрасываем флаг завершения, если добавили новую задачу
        if (allTasksCompleted_) {
            allTasksCompleted_ = false;
        }

        queueCV_.notify_one();
    }
}

bool Spider::shouldProcessUrl(const std::string& url, int depth) {
    if (depth >= config_.getSpiderMaxDepth()) {
        return false;
    }

    std::string startDomain = getDomainFromUrl(config_.getSpiderStartUrl());
    std::string currentDomain = getDomainFromUrl(url);

    std::string cleanStartDomain = startDomain;
    if (cleanStartDomain.find("www.") == 0) {
        cleanStartDomain = cleanStartDomain.substr(4);
    }

    std::string cleanCurrentDomain = currentDomain;
    if (cleanCurrentDomain.find("www.") == 0) {
        cleanCurrentDomain = cleanCurrentDomain.substr(4);
    }

    if (cleanCurrentDomain != cleanStartDomain) {
        return false;
    }

    static const std::vector<std::string> skipExtensions = {
        ".svg", ".png", ".jpg", ".jpeg", ".gif", ".bmp", ".ico",
        ".css", ".js", ".woff", ".woff2", ".ttf", ".eot",
        ".pdf", ".zip", ".rar", ".7z", ".exe", ".dll",
        ".mp3", ".mp4", ".avi", ".mov", ".wmv", ".apk", ".json"
    };

    for (const auto& ext : skipExtensions) {
        if (url.size() >= ext.size() &&
            url.compare(url.size() - ext.size(), ext.size(), ext) == 0) {
            return false;
        }
    }

    std::lock_guard<std::mutex> lock(queueMutex_);
    return visitedUrls_.find(url) == visitedUrls_.end();
}

std::string Spider::getDomainFromUrl(const std::string& url) const {
    size_t start = url.find("://");
    if (start == std::string::npos) return "";
    start += 3;

    size_t end = url.find("/", start);
    if (end == std::string::npos) return url.substr(start);

    return url.substr(start, end - start);
}

void Spider::processUrl(const std::string& url, int depth) {
    if (depth >= config_.getSpiderMaxDepth()) {
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    try {
        std::string contentType;
        std::string content = client_.downloadPage(url, contentType);

        if (content.empty()) {
            std::cerr << "Не удалось загрузить: " << url << std::endl;
            return;
        }

        if (content.size() < 1000) {
            return;
        }

        if (contentType.find("text/html") == std::string::npos) {
            return;
        }

        if (indexer_.indexDocument(url, url, content, contentType)) {
            processedCount_++;
        }
        else {
            std::cerr << "ОШИБКА индексации: " << url << std::endl;
            return;
        }

        auto links = client_.extractLinks(content, url);
        for (const auto& link : links) {
            if (shouldProcessUrl(link, depth + 1)) {
                addUrlToQueue(link, depth + 1);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка обработки URL " << url << ": " << e.what() << std::endl;
    }
}