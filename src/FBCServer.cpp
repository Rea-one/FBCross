#include "FBCServer.h"


FBCServer::FBCServer(const std::string& config_path)
{
    config = std::move(get_config(config_path));
}