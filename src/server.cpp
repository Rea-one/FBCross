#include "server.h"


using io_type = std::unordered_map<std::string, std::shared_ptr<IOMessage>>;


Server::Server(const std::string& config_path)
{
    config_ = std::move(get_config(config_path));
    server_ip_ = config_.get<std::string>("server.ip");
    server_port_ = config_.get<int>("server.port");
    database_ip_ = config_.get<std::string>("database.postgresql.ip");
    database_port_ = config_.get<int>("database.postgresql.port");
    database_name_ = config_.get<std::string>("database.postgresql.dbname");
    database_user_ = config_.get<std::string>("database.postgresql.user");
    database_password_ = config_.get<std::string>("database.postgresql.password");

    receive_limit_ = config_.get<int>("server.receive_limit");
    work_limit_ = config_.get<int>("server.work_limit");


    io_mes_ = std::make_shared<io_type>();
    fore_pool_ = std::make_shared<LimPool> (receive_limit_);
    back_pool_ = std::make_shared<PQPool> (work_limit_, database_ip_, database_port_,
        database_name_, database_user_, database_password_);
    listener_ = std::make_unique<Listener> (server_ip_, server_port_);
    receivers_ = std::make_unique<Receivers> (receive_limit_, fore_pool_, io_mes_);
    workers_ = std::make_unique<Workers> (work_limit_, back_pool_, io_mes_);
}


void Server::start()
{
    std::cerr << "Server 启动中" << std::endl;
    
    workers_ -> start();
    std::cerr << "Workers 已启动" << std::endl;

    // receivers_ -> start();
    // std::cerr << "Receivers 已启动" << std::endl;

    // listener_ -> start();
    // std::cerr << "Listener 已启动" << std::endl;
    // while (true)
    // {
    //     boost::this_thread::sleep_for(boost::chrono::seconds(1));
    // }
}