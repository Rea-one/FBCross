#pragma once

#include <mutex>
#include <shared_mutex>
#include <queue>


#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "pqxx/pqxx"

#include "to_sql.h"
#include "IOmessage.h"



class MessPasser
{

private:
    std::string sender_{};
    std::string receiver_{};
    std::string message_{};

private:
    std::shared_ptr<IOMessage> io_message_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool active_{};
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    int id_;
    std::shared_mutex counter_mutex_;
    int task_count_{};
    std::queue<std::function<std::string()>> tasks_;
    bool stop_{};
    boost::thread thread_;
    // std::list<std::shared_ptr<Receiver>> group_;

public:
    void start();

    void stop();
    bool link(std::shared_ptr<boost::asio::ip::tcp::socket>&& socket);
    bool mlink(std::shared_ptr<IOMessage>&& io_message);
    void input();
    void output();

private:
    // void pick_up();
};