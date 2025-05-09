#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>



#include "methods.h"
#include "threads.h"

#include "connect_pool.h"
#include "IOmessage.h"
#include "listener.h"
#include "receivers.h"
#include "workers.h"

using io_type = std::unordered_map<std::string, std::shared_ptr<IOMessage>>;
class Server
{
private:
    bool stop_ = false;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::string server_ip_;
    int server_port_;
    std::string database_ip_;
    int database_port_;
    std::string database_name_;
    std::string database_user_;
    std::string database_password_;

    int receive_limit_;
    int work_limit_;

    std::shared_ptr<LimPool> fore_pool_{};
    std::shared_ptr<PQPool> back_pool_{};
    std::shared_ptr<io_type> io_mes_{};

    std::unique_ptr<Listener> listener_{};
    std::unique_ptr<Receivers> receivers_{};
    std::unique_ptr<Workers> workers_{};
    boost::property_tree::ptree config_;

public:
    Server(const std::string& config_path);
    void start();
};