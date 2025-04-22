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
    server_.Get("/test", [](const httplib::Request &req,
                            httplib::Response &res)
                { res.set_content("Connected!", "text/plain"); });

    server_.listen(user_ip_ + ":" + user_port_);
}