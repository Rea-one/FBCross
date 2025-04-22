#pragma once

#include <vector>
#include <string>
#include <thread>
#include <mutex>

#include "to_sql.h"

#include "threads.h"

class Workers
{
private:
    int current_workers_;
    int worker_limit_;
    Threads workers_;

    std::mutex mutex_;

    std::string wait_for_message();

    Workers() = delete;

public:
    Workers(const int& worker_limit);

};
