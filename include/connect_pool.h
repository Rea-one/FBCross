#pragma once

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include "methods.h"

class ConnectPool
{
private:
    int current_connections_;
    int connect_limit_;

    std::queue<boost::asio::ip::tcp::socket> pool_;
public:
    ConnectPool(const int& connect_limit);
    bool submit(boost::asio::ip::tcp::socket&& socket);
    boost::asio::ip::tcp::socket get();
    bool is_full();
    bool is_empty();
};
