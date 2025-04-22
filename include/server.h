#pragma once

#include <vector>
#include <string>
#include <utility>
#include <boost/property_tree/ptree.hpp>


#include "httplib.h"


#include "methods.h"
#include "threads.h"

#include "listener.h"
#include "connect_pool.h"
#include "workers.h"


class Server
{
private:

    std::string sever_ip_;
    int sever_port_;
    std::string database_ip_;
    int database_port_;

    int connection_limit_;
    int worker_limit_;

    std::unique_ptr<Listener> listener_{};
    std::unique_ptr<ConnectPool> pool_{};
    std::unique_ptr<Workers> workers_{};

    boost::property_tree::ptree config_;

public:
    Server(const std::string& config_path);
    void start();
};