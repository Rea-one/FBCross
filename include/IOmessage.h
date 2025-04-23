#pragma once

#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <unordered_map>
#include <condition_variable>


class IOMesaage
{
public:
    std::mutex Imutex_;
    std::condition_variable Icond_;
    std::queue<std::pair<std::string, std::string>> Iqueue_;
    std::mutex Omutex_;
    std::condition_variable Ocond_;
    std::unordered_map<std::string, std::queue<std::string>> Oqueue_;


    IOMesaage(const IOMesaage&) = delete;
    IOMesaage& operator=(const IOMesaage&) = delete;

    bool is_empty_I();
    bool is_empty_O();

    void write_o(std::string& ip, std::string& message);
    void write_i(std::string& message);
    std::string read_o(std::string& ip);
    std::pair<std::string, std::string> read_i();
};