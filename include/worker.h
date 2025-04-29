#pragma once

#include <utility>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <list>


#include "boost/asio.hpp"
#include "boost/thread.hpp"

#include <dbng.hpp>
#include <postgresql.hpp>

#include "to_sql.h"
#include "IOmessage.h"



class Worker
{

private:
    std::string sender_{};
    std::string receiver_{};
    std::string message_{};

private:
    std::shared_ptr<IOMessage> io_message_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool active_{};
    std::unique_ptr<pqxx::connection> conn_;
    int id_;
    std::shared_mutex counter_mutex_;
    int task_count_{};
    std::queue<std::function<std::string()>> tasks_;
    bool stop_{};
    boost::thread thread_;
    // std::list<std::shared_ptr<Worker>> group_;

public:
    void start();

    void stop();
    bool link(std::unique_ptr<pqxx::connection>&& conn);
    bool mlink(std::shared_ptr<IOMessage>&& io_message);
    bool submit(std::function<std::string()>&& task);
    std::string operation(std::string message);

private:
    // void pick_up();
};