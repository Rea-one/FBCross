#pragma once

#include <vector>
#include <string>
#include <utility>
#include <boost/property_tree/ptree.hpp>


#include "httplib.h"


#include "methods.h"
#include "threads.h"

#include "connect_pool.h"
#include "listener.h"
#include "receiver.h"
#include "workers.h"


class Server
{
private:
    bool stop_ = false;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::string sever_ip_;
    int sever_port_;
    std::string database_ip_;
    int database_port_;

    int receive_limit_;
    int work_limit_;

    std::shared_ptr<LimPool> fore_pool_{};
    std::shared_ptr<PQPool> back_pool_{};

    std::unique_ptr<Listener> listener_{};
    std::unique_ptr<Receiver> receivers_{};
    std::unique_ptr<Workers> workers_{};
    boost::property_tree::ptree config_;

public:
    Server(const std::string& config_path);
    void start();
};