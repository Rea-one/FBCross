
#include "server.h"


int main()
{
    Server server("config.json");
    server.start();
}