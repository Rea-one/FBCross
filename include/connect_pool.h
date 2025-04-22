#pragma once

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <boost/property_tree/json_parser.hpp>
#include "httplib.h"

#include "methods.h"


class ConnectPool
{
public:
    ConnectPool(const std::string& config_path);
    ~ConnectPool();

    httplib::Client getConnection();

    void releaseConnection(httplib::Client conn);

private:
    std::string db_url_;
    int max_connections_;
    int current_connections_;

    // 连接队列和同步机制
    std::queue<httplib::Client> connection_queue_;
    std::mutex mtx_;
    std::condition_variable cond_;
};