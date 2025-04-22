#pragma once

#include <string>

#include <boost/property_tree/json_parser.hpp>

#include "httplib.h"

#include "methods.h"

class Listener
{
public:
    Listener(const std::string& config_path);
    ~Listener();

    void Run();

private:
    std::string user_ip_;
    std::string user_port_;
    httplib::Client data_base_;
    httplib::Server server_;
};