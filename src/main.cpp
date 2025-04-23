
#include "server.h"


int main()
{
    Server server("config/config.json");
    server.start();
}