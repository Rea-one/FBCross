#pragma once

#include <string>

#include <boost/property_tree/json_parser.hpp>

#include "httplib.h"

class Listener
{
public:
    Listener(std::string &config_path);
    ~Listener();

    void Run();

private:
    std::string user_ip_;
    std::string user_port_;
    httplib::Server svr;
};