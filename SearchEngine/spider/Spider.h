//Spider.h
#pragma once
#include <queue>
#include <unordered_set>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "../common/Configuration.h"
#include "../common/DatabaseManager.h"
#include "../common/Indexer.h"
#include "HttpClient.h"

class Spider {
public:
    Spider(const Configuration& config);
    ~Spider();

    void start();
    void stop();
    void waitForCompletion();

private:
    void workerThread();
    void processUrl(const std::string& url, int depth);
    bool shouldProcessUrl(const std::string& url, int depth);
    std::string getDomainFromUrl(const std::string& url) const;
    void addUrlToQueue(const std::string& url, int depth);

    Configuration config_;
    DatabaseManager dbManager_;
    Indexer indexer_;
    HttpClient client_;

    // Многопоточная очередь
    std::queue<std::pair<std::string, int>> urlQueue_;
    std::unordered_set<std::string> visitedUrls_;
    std::mutex queueMutex_;
    std::condition_variable queueCV_;

    // Пул потоков
    std::vector<std::thread> workers_;
    std::atomic<bool> stopFlag_{ false };
    std::atomic<int> activeWorkers_{ 0 };
    std::atomic<int> processedCount_{ 0 };
    std::atomic<int> activeTasks_{ 0 };
    std::atomic<bool> allTasksCompleted_{ false };

    // Guard для progress thread
    std::thread progressThread_;
    std::atomic<bool> stopProgress_{ false };
};