#pragma once

#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <condition_variable>


class IOMessage
{
private:
    std::shared_mutex Imutex_;
    std::condition_variable_any Icond_;
    std::shared_mutex Omutex_;
    std::condition_variable_any Ocond_;

public:
    
    std::queue<std::string> Iqueue_;

    std::queue<std::string> Oqueue_;


    bool is_empty_I();
    bool is_empty_O();

    void write_o(std::string& message);
    void write_i(std::string& message);
    std::string read_o();
    std::string read_i();
    std::shared_mutex& in_lock();
    std::shared_mutex& out_lock();
    std::condition_variable_any& in_cond();
    std::condition_variable_any& out_cond();
};