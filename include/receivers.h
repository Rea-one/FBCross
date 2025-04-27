#pragma once

#include <string>
#include <vector>
#include <queue>

#include <boost/asio.hpp>

#include "threads.h"
#include "IOmessage.h"
#include "connect_pool.h"
#include "mess_passer.h"

class frm_handler
{
public:
    frm_handler(std::shared_ptr<IOMessage> io_mes);
    void get_message(boost::asio::ip::tcp::socket &socket);
    void put_message(boost::asio::ip::tcp::socket &socket);

private:
    std::shared_ptr<IOMessage> io_mes_;
};


using io_type = std::unordered_map<std::string, std::shared_ptr<IOMessage>>;

class Receivers
{
private:
    int current_passer_;
    int passer_limit_;
    std::priority_queue<std::shared_ptr<MessPasser>> passers_;

    std::shared_ptr<LimPool> pool_;
    std::shared_ptr<io_type> io_mes_;


public:
    Receivers(const int &worker_limit, std::shared_ptr<LimPool> pool, std::shared_ptr<io_type> io_mes);
    void start();
};