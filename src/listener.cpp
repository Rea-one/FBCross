#include "listener.h"

Listener::Listener(std::string &config_path)
{
    boost::property_tree::ptree config;
    boost::property_tree::read_json(config_path, config);

    auto ip = config.get<std::string>("nets.0.ip");
    auto port = config.get<int>("nets.0.port");
}