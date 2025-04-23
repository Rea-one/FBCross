#pragma once

#include <string>
#include <vector>

#include "threads.h"
#include "IOmessage.h"
#include "connect_pool.h"

class frm_handler
{
public:
    frm_handler(std::shared_ptr<IOMessage> io_mes);
    void get_message(boost::asio::ip::tcp::socket &socket);
    void put_message(boost::asio::ip::tcp::socket &socket);

private:
    std::shared_ptr<IOMessage> io_mes_;
};

class Receiver
{

private:
    int receiver_limit_;

    Threads receivers_;

    std::shared_ptr<LimPool> pool_;
    std::shared_ptr<IOMessage> io_mes_;
    std::vector<std::queue<std::string>> queues_;
    std::unordered_map<std::string, int> message_index_;

public:
    Receiver(const int &receiver_limit, std::shared_ptr<LimPool> pool, std::shared_ptr<IOMessage> io_mes);
    void start();
    void wait();
};