#pragma once

#include <string>
#include <vector>

#include <boost/property_tree/json_parser.hpp>

class ConnectPool
{
public:
    ConnectPool(const std::string& config_file);
    ~ConnectPool();

    bool GetConnect(std::string& ip, int& port);

private:
    void LoadConfig(const std::string& config_file);

private:
    std::vector<std::pair<std::string, int>> m_connects;
    int m_index;
};