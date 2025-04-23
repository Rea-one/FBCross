#pragma once

#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <memory>


#include <boost/asio.hpp>

#include "connect_pool.h"
#include "IOmessage.h"

#include "to_sql.h"

#include "threads.h"

class bkm_handler
{
public:
    bkm_handler(std::shared_ptr<IOMesaage> io_mes);
    std::string get_message();
    std::string operation(std::string message, boost::asio::ip::tcp::socket& socket);
    void put_message(std::string& message);
private:
    std::shared_ptr<IOMesaage> io_mes_;
    std::string user_ip_;
};

class Workers
{
private:
    int current_workers_;
    int worker_limit_;
    Threads workers_;

    std::shared_ptr<ExpPool> pool_;
    std::shared_ptr<IOMesaage> io_mes_;

    Workers() = delete;

public:
    Workers(const int &worker_limit, ExpPool& pool, IOMesaage& io_mes);
    void start();
    void wait();
};
