#include "connect_pool.h"

ConnectPool::ConnectPool(const std::string &config_path)
{
    boost::property_tree::ptree config;
    boost::property_tree::read_json(config_path, config);

    db_url_ = get_url(config, 1);
    max_connections_ = config.get<int>("nets.1.max_connections"); // 最大连接数

    // 初始化连接池（预创建部分连接）
    for (int i = 0; i < max_connections_; ++i)
    {
        httplib::Client conn(db_url_);
        connection_queue_.push(conn);
    }
    current_connections_ = max_connections_;
}

ConnectPool::~ConnectPool()
{
}

httplib::Client ConnectPool::getConnection()
{
    std::unique_lock<std::mutex> lock(mtx_);
    // 等待可用连接
    while (connection_queue_.empty())
    {
        cond_.wait(lock);
    }

    // 取出一个连接
    httplib::Client conn = connection_queue_.front();
    connection_queue_.pop();
    current_connections_--;
    return conn;
}

void ConnectPool::releaseConnection(httplib::Client conn)
{
    std::unique_lock<std::mutex> lock(mtx_);
    // 将连接放回连接池
    connection_queue_.push(conn);
    current_connections_++;
    // 唤醒等待的线程
    cond_.notify_one();
}