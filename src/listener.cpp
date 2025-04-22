#include "listener.h"

Listener::Listener(const std::string &config_path) : data_base_(get_url(config_path, 1))
{
    boost::property_tree::ptree config;
    boost::property_tree::read_json(config_path, config);
    user_ip_ = config.get<std::string>("nets.0.ip");
    user_port_ = config.get<std::string>("nets.0.port");
}

Listener::~Listener()
{
    server_.stop();
    data_base_.stop();
}

void Listener::Run()
{
    // 注册路由（示例：处理 GET 请求）
    server_.Get("/test", [&](const httplib::Request &req,
                             httplib::Response &res)
                {
        httplib::Client db_conn = db_pool_.getConnection();
        auto resp = db_conn.Get("/data");  // 假设数据库有 "/data" 接口

        if (resp && resp -> status == 200)
        {
            res.set_content(resp -> body, "application/json");
        }
        else
        {
            res.status = 500;
            res.set_content("Database error", "text/plain");
        }

        // 释放连接回池
        db_pool_.releaseConnection(std::move(db_conn)); });

    server_.listen(user_ip_, std::stoi(user_port_));
}