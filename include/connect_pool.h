#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <shared_mutex>
#include <condition_variable>

#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <pqxx/pqxx>

#include "methods.h"


class CONNECT_POOL
{
protected:
    int current_connections_;
    int connect_limit_;
    boost::asio::io_context io_context_;

    std::mutex pool_mutex_;
    std::condition_variable pool_cv_;

    std::queue<boost::asio::ip::tcp::socket> pool_;
public:
    CONNECT_POOL(int connect_limit);
    boost::asio::io_context& get_io_context();
    bool submit(boost::asio::ip::tcp::socket&& socket);
    boost::asio::ip::tcp::socket get();
    bool is_full();
    bool is_empty();

    template <typename Func>
    auto luck (Func&& func)
    {
        std::unique_lock<std::mutex> lock(pool_mutex_);
        return func();
    }
    template <typename Func>
    auto lsck (Func&& func)
    {
        std::shared_lock<std::mutex> lock(pool_mutex_);
        return func();
    }
};

class ExpPool: public CONNECT_POOL
{
private:
    std::string ip_;
    int port_;

public:
    ExpPool(int connect_limit, std::string ip, int port);
};

class LimPool: public CONNECT_POOL
{
private:

public:
    LimPool(int connect_limit);
};


class PQPool
{
    
private:
    std::string ip_;
    int port_;
    std::string dbname_;
    std::string user_;
    std::string password_;

    std::atomic<int> current_connections_;
    std::atomic<int> connect_limit_;

    std::mutex pool_mutex_;
    std::condition_variable pool_cv_;

    std::queue<pqxx::connection> pool_;
public:
    PQPool(int connect_limit, std::string ip, int port,
        std::string dbname, std::string user, std::string password);
    bool submit(pqxx::connection&& conn);
    pqxx::connection get();
    bool is_full();
    bool is_empty();
    bool over_size();
    bool float_size();

    std::mutex& pool_lock();
    std::condition_variable& pool_cv();
};