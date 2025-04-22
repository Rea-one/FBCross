#include "threads.h"

Threads::Threads(size_t thread_count)
    :stop_(false)
{
    for (size_t i = 0; i < thread_count; ++i)
    {
        threads_.emplace_back([this]
                              { workerThread(); });
    }
}

Threads::~Threads()
{
    {
        std::unique_lock<std::mutex> lock(mtx_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto &t : threads_)
    {
        t.join();
    }
}

void Threads::workerThread()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this]
                     { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty())
                return;
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}

template <typename Func, typename... Args>
auto Threads::submit(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
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