#include "workers.h"

Workers::Workers(const int& worker_limit):
    worker_limit_(worker_limit), current_workers_(0),
{
}


std::string Workers::wait_for_message()
{
    std::unique_lock<std::mutex> lock(mutex_);
}