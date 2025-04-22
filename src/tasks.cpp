#include "tasks.h"

Tasks::Tasks()
{

}

bool Tasks::add_task(httplib::Server& user, ConnectPool& pool)
{
    httplib::Client conn = pool.getConnection();
    task_pool.push_back(std::move(user, conn));
    return true;
}