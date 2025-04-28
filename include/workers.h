#pragma once


#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <queue>


#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "connect_pool.h"
#include "IOmessage.h"

#include "to_sql.h"

#include "worker.h"
#include "pqxx/pqxx"


// class bkm_handler
// {
// public:
//     bkm_handler(std::shared_ptr<IOMessage> io_mes);
//     std::string get_message();
//     std::string operation(std::string message, pqxx::connection& conn);
//     void put_message(std::string& message);
// private:
//     std::shared_ptr<IOMessage> io_mes_;
//     std::string user_ip_;


//     std::string sender;
//     std::string receiver;
//     std::string pmessage;
//     std::string task;
// };

using io_type = std::unordered_map<std::string, std::shared_ptr<IOMessage>>;

class Workers
{
private:
    int current_workers_;
    int worker_limit_;
    std::priority_queue<std::shared_ptr<Worker>> workers_;

    std::shared_ptr<PQPool> pool_;
    std::shared_ptr<io_type> io_mes_;

    Workers() = delete;

public:
    Workers(const int &worker_limit, std::shared_ptr<PQPool> pool, std::shared_ptr<io_type> io_mes);
    void start();
};
