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

    bool is_empty();
    bool is_full();
    bool over_size();
    bool float_size();

    template <typename Func, typename... Args>
    auto submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
    {
        using return_type = decltype(func(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx_);
            tasks_.emplace([task]()
                        { (*task)(); });
        }
        cv_.notify_one();
        return res;
    }
private:
    void workerThread();
};