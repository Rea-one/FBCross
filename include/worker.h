#pragma once

#include <utility>
#include <mutex>
#include <queue>
#include <list>


#include "boost/asio.hpp"
#include "boost/thread.hpp"

#include "pqxx/pqxx"


class Worker
{

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    bool active_{};
    std::unique_ptr<pqxx::connection> conn_;
    int id_;
    int task_count_{};
    std::queue<std::function<std::string()>> tasks_;
    bool stop_{};
    boost::thread thread_;

public:
    void start();

    void stop();
    bool link(std::unique_ptr<pqxx::connection>&& conn);
    bool submit(std::function<std::string()>&& task);
};