#include "server.h"


Server::Server(const std::string& config_path)
{
    config_ = std::move(get_config(config_path));
    sever_ip_ = config_.get<std::string>("server.ip");
    sever_port_ = config_.get<int>("server.port");
    database_ip_ = config_.get<std::string>("database.postgresql.ip");
    database_port_ = config_.get<int>("database.postgresql.port");

    connection_limit_ = config_.get<int>("server.connection_limit");
    worker_limit_ = config_.get<int>("server.worker_limit");

    listener_ = std::make_unique<Listener> (sever_ip_, sever_port_);
    pool_ = std::make_unique<ConnectPool> (connection_limit_);
    workers_ = std::make_unique<Workers> (worker_limit_);
}


void Server::start()
{
    workers_->start();
    pool_->start();
    listener_->start();
}