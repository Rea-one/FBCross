#pragma once

#include "string"

#include <boost/property_tree/json_parser.hpp>


std::string get_url(const boost::property_tree::ptree& config, int index);


std::string get_url(const std::string& config_path, int index);


boost::property_tree::ptree get_config(const std::string& config_path);
