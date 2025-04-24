#pragma once
#include <vector>
#include <string>
#include <unordered_map>
// #include <spdlog/spdlog.h>

#include "methods.h"
#include "connect_pool.h"


// enum class RequestType
// {
//     QUE,
//     INS,
//     UPD,
//     DEL,
//     SEL
// };


struct ChatMessage
{
    std::string sender;
    std::string receiver;
    std::string message;
};

// struct RequestMessage
// {
//     std::string buzhidao;
// };

// struct Request
// {
//     RequestType type;
//     RequestMessage message;
// };

// class DatabaseUtils
// {
// public:
//     static std::string executeQuery(const std::string& query, ConnectPool& pool);
//     static bool executeInsert(const std::string& data, ConnectPool& pool);
// };


class DBU
{
private:
    std::string escape_sql(const std::string& str);
public:
    DBU(std::string config_path);
    ~DBU();

    bool insertMessage(const ChatMessage& message, ConnectPool& pool);
    bool exisitsUser(const std::string& username, ConnectPool& pool);
    bool checkUser(const std::string& username, const std::string& password, ConnectPool& pool);
    bool registerUser(const std::string& username, const std::string& password, ConnectPool& pool);

};

class DBU_Worker
{
    std::string escape_sql(const std::string& str);
public:
    bool insertMessage(const ChatMessage& message, ConnectPool& pool);
    bool exisitsUser(const std::string& username, ConnectPool& pool);
    bool checkUser(const std::string& username, const std::string& password, ConnectPool& pool);
    bool registerUser(const std::string& username, const std::string& password, ConnectPool& pool);
};