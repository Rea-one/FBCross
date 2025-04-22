#pragma once

#include <string>
#include <queue>
#include <future>
#include <mutex>

#include "httplib.h"

#include "connect_pool.h"

class Tasks
{
public:
    std::mutex mutex_;
    Tasks();

    bool add_task(httplib::Server& user, ConnectPool& pool);

private:
    std::queue<std::pair<httplib::Server, httplib::Client> task_pool;
};