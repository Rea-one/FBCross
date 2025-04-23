#pragma once
#include <vector>
#include <queue>
#include <future>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <boost/thread.hpp>

class Threads
{
private:
    std::atomic<int> tasks_in_progress_{0};
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;

public:
    explicit Threads(size_t thread_count = 4);
    ~Threads();

    void wait();
    template <typename Func, typename... Args>
    auto submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>;

private:
    void workerThread();
};