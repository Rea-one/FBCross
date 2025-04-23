#pragma once

#include "string"

#include <boost/property_tree/json_parser.hpp>


std::string get_url(const boost::property_tree::ptree& config, int index)
{
    return "http://" + config.get<std::string>("nets." + std::to_string(index) + ".ip")
    + ":" + config.get<std::string>("nets." + std::to_string(index) + ".port");
}


std::string get_url(const std::string& config_path, int index)
{
    boost::property_tree::ptree config;
    boost::property_tree::read_json(config_path, config);

    return get_url(config, index);
}

boost::property_tree::ptree get_config(const std::string& config_path)
{
    boost::property_tree::ptree config;
    boost::property_tree::read_json(config_path, config);

    return config;
}
