#include "listener.h"

Listener::Listener(const std::string &config_path): db_pool_(config_path)
{
    boost::property_tree::ptree config;
    boost::property_tree::read_json(config_path, config);
    user_ip_ = config.get<std::string>("nets.0.ip");
    user_port_ = config.get<std::string>("nets.0.port");
}

Listener::~Listener()
{
    server_.stop();
}

void Listener::Run(ConnectPool& pool, Tasks& tasks)
{
    server_thread_ = std::thread([this, &pool, &tasks]()
    {
        server_.listen(user_ip_, std::stoi(user_port_));
        auto target = pool.getConnection();
        if (tasks.add_task(std::move(server_), pool))
    });
}