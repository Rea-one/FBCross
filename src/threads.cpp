#include "threads.h"

Threads::Threads(size_t thread_count)
    : stop_(false)
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
            // 等待任务队列非空或线程需终止
            cv_.wait(lock, [this]
                     { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty())
            {
                return; // 线程退出
            }
            task = std::move(tasks_.front());
            tasks_.pop();
            tasks_in_progress_++; // 开始执行任务前计数器+1
        }
        task();               // 执行任务
        tasks_in_progress_--; // 任务完成后计数器-1
    }
}

void Threads::wait()
{
    std::unique_lock<std::mutex> lock(mtx_);
    // 等待队列空且无任务在运行
    cv_.wait(lock, [this]
             { return tasks_.empty() && tasks_in_progress_ == 0; });
}