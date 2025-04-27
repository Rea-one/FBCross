#include "worker.h"

void Worker::start()
{
    thread_ = boost::thread([this]()
    {
        while (!stop_)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            active_ = true;
            if (!conn_)
                cv_.wait(lock);
            if (!tasks_.empty())
            {
                std::string message = tasks_.front()();
                tasks_.pop();
                pqxx::work txn(*conn_);
                txn.exec(message);
                txn.commit();
            }
        }
    });
}

void Worker::stop()
{
    active_ = false;
    stop_ = true;
    cv_.notify_one();
    thread_.join();
}

bool Worker::link(std::unique_ptr<pqxx::connection>&& conn)
{
    if (conn_)
        return false;
    else
    {
        std::unique_lock<std::mutex> lock(mutex_);
        conn_ = std::move(conn);
        cv_.notify_one();
        return true;
    }
}

bool Worker::submit(std::function<std::string()>&& task)
{
    if (!active_)
        return false;
    else
    {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
        cv_.notify_one();
        return true;
    }
}