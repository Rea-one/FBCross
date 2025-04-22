#include "connect_pool.h"


ConnectPool::ConnectPool(const int& connect_limit):
    connect_limit_(connect_limit), current_connections_(0)
{
}


bool ConnectPool::submit(boost::asio::ip::tcp::socket&& socket)
{
    if (current_connections_ >= connect_limit_)
        return false;
    pool_.emplace(socket);
    current_connections_ ++;
}

boost::asio::ip::tcp::socket ConnectPool::get()
{
    if (pool_.empty())
    {
        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address("127.0.0.1"), 8080);
        return boost::asio::ip::tcp::socket(io_context_, endpoint);
    }
    auto socket = std::move(pool_.front());
    pool_.pop();
    current_connections_ --;
    return socket;
}


bool ConnectPool::is_full()
{
    return current_connections_ >= connect_limit_;
}

bool ConnectPool::is_empty()
{
    return pool_.empty();
}