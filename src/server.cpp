#include "server.h"


Server::Server(const std::string& config_path)
{
    config_ = std::move(get_config(config_path));
    sever_ip_ = config_.get<std::string>("server.ip");
    sever_port_ = config_.get<int>("server.port");
    database_ip_ = config_.get<std::string>("database.postgresql.ip");
    database_port_ = config_.get<int>("database.postgresql.port");

    receive_limit_ = config_.get<int>("server.receive_limit");
    work_limit_ = config_.get<int>("server.work_limit");

    

    fore_pool_ = std::make_shared<LimPool> (receive_limit_);
    back_pool_ = std::make_shared<PQPool> (work_limit_, database_ip_, database_port_);
    io_mes_ = std::make_shared<IOMessage> ();
    listener_ = std::make_unique<Listener> (sever_ip_, sever_port_);
    receivers_ = std::make_unique<Receiver> (receive_limit_, fore_pool_, io_mes_);
    workers_ = std::make_unique<Workers> (work_limit_, back_pool_, io_mes_);
}


void Server::start()
{
    workers_ -> start();
    receivers_ -> start();
    listener_ -> start();
    while (true)
    {
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }
}