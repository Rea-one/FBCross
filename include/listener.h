#pragma once

#include <string>
#include <chrono>
#include <boost/property_tree/json_parser.hpp>

#include "httplib.h"

#include "connect_pool.h"
#include "tasks.h"
#include "methods.h"

class Listener
{
public:
    Listener(const std::string& config_path);
    ~Listener();

    void Run(ConnectPool& pool, Tasks& tasks);  // 启动监听线程
    void Stop(); // 停止服务

private:
    std::thread server_thread_;  // 保存监听线程
    bool running_;               // 标志是否运行
    std::string user_ip_;
    std::string user_port_;
    ConnectPool db_pool_;
    httplib::Server server_;
};