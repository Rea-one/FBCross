#pragma once


#include "listener.h"
#include "database_utils.h"


int main()
{
    Listener listener("./config/config.json");
    listener.Run();
    return 0;
}