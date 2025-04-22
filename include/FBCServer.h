#pragma once

#include "boost/property_tree/ptree.hpp"

#include "methods.h"
#include "listener.h"
#include "threads.h"


class FBCServer
{
public:
    FBCServer(const std::string& config_path);
    void Run();
private:
    boost::property_tree::ptree config;
    std::unique_ptr<Listener> listener;
    std::unique_ptr<Threads> workers;
};