#include "connect_pool.h"

CONNECT_POOL::CONNECT_POOL(int connect_limit):
    connect_limit_(connect_limit),
    current_connections_(0)
{}
boost::asio::io_context& CONNECT_POOL::get_io_context() { return io_context_; };


bool CONNECT_POOL::submit(boost::asio::ip::tcp::socket &&socket)
{
    if (current_connections_ >= connect_limit_)
        return false;
    pool_.emplace(std::move(socket));
    current_connections_++;
    pool_cv_.notify_one();
    return true;
}

boost::asio::ip::tcp::socket CONNECT_POOL::get()
{
    if (pool_.empty())
    {
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address("127.0.0.1"), 8080);
        return boost::asio::ip::tcp::socket(io_context_, endpoint);
    }
    auto socket = std::move(pool_.front());
    pool_.pop();
    current_connections_--;
    return socket;
}

bool CONNECT_POOL::is_full()
{
    return current_connections_ >= connect_limit_;
}

bool CONNECT_POOL::is_empty()
{
    return pool_.empty();
}

ExpPool::ExpPool(int connect_limit, std::string ip, int port):
    CONNECT_POOL(connect_limit), ip_(ip), port_(port)
{
    boost::asio::ip::tcp::resolver resolver(io_context_);
    auto endpoint = resolver.resolve(ip_, std::to_string(port));
    for (int i = 0; i < connect_limit_; i++)
    {
        boost::asio::ip::tcp::socket socket(io_context_);
        boost::asio::connect(socket, endpoint);
        pool_.emplace(std::move(socket));
    }
}


LimPool::LimPool(int connect_limit):
    CONNECT_POOL(connect_limit)
{
}


PQPool::PQPool(int connect_limit, std::string ip, int port,
    std::string dbname, std::string user, std::string password):
    pool_mutex_(), pool_cv_(),
    current_connections_(0),  // 初始化连接计数器
    connect_limit_(connect_limit), ip_(ip), port_(port),
    dbname_(dbname), user_(user), password_(password)
{
    std::string conn_str =
        "host=" + ip_ +
        " port=" + std::to_string(port_) +
        " dbname=" + dbname_ +
        " user=" + user_ +
        " password=" + password_;
    for (int order = 0; order < connect_limit_; order++)
    {
        pqxx::connection conn(conn_str);
        pool_.emplace(std::move(conn));
        current_connections_ ++;
        std::cout << "成功创建第" << current_connections_ << "个链接" << std::endl;
    }
}

bool PQPool::submit(pqxx::connection &&conn)
{
    if (current_connections_ >= connect_limit_)
        return false;
    pool_.emplace(std::move(conn));
    current_connections_ ++;
    pool_cv_.notify_one();
    return true;
}

pqxx::connection PQPool::get()
{
    if (pool_.empty())
    {
        if (current_connections_ >= connect_limit_)
        {
            throw std::runtime_error("连接池满了，不允许再请求连接");
        }
        pqxx::connection conn("host=" + ip_ + 
            " port=" + std::to_string(port_) +
            " dbname=" + dbname_ +
            " user=" + user_ +
            " password=" + password_);
        current_connections_ ++;
        return conn;
    }
    else
    {
        auto conn = std::move(pool_.front());
        pool_.pop();
        current_connections_--;
        return conn;
    }
}

bool PQPool::is_full()
{
    return current_connections_ >= connect_limit_;
}

bool PQPool::is_empty()
{
    return pool_.empty();
}



bool PQPool::over_size()
{
    return current_connections_ > connect_limit_;
}

bool PQPool::float_size()
{
    return current_connections_ >= connect_limit_;
}

std::mutex& PQPool::pool_lock()
{
    return pool_mutex_;
}

std::condition_variable& PQPool::pool_cv()
{
    return pool_cv_;
}