#include "database_utils.h"

// std::string DatabaseUtils::executeQuery(const std::string &query, ConnectPool &pool)
// {
//     std::string result;
//     httplib::Client conn = pool.getConnection();

//     auto response = conn.Post(
//         "SELECT " + query + ";",
//     );

//     if (response && response->status == 200)
//     {
//         result = response->body;
//     }

//     pool.releaseConnection(conn);

//     return result;
// }

// bool DatabaseUtils::executeInsert(const std::string &data, ConnectPool &pool)
// {
//     httplib::Client conn = pool.getConnection();

//     auto response = conn.Post(
//         "INSERT INTO chat_messages (message) VALUES ('" + data + "');"
//     );

//     pool.releaseConnection(conn);
//     return response && response->status == 200;
// }

DBU::DBU(std::string config_path)
{
    auto config = std::move(get_config(config_path));
}

DBU::~DBU() {}

bool DBU::insertMessage(const ChatMessage &message, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "INSERT INTO MCrossMDB (sender, receiver, message, timestamp) VALUES ("+ escape_sql(message.sender) + "," + escape_sql(message.receiver) + "," + escape_sql(message.message) + ", now());");
    pool.releaseConnection(conn);

    return response && response->status == 200;
}

bool DBU::exisitsUser(const std::string &username, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "SELECT * FROM MCrossUDB WHERE username = " + escape_sql(username) + ";");

    pool.releaseConnection(conn);

    return response && response->status == 200;
}

bool DBU::checkUser(const std::string &username, const std::string &password, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "SELECT * FROM MCrossUDB WHERE username = '" + username + "' AND password = '" + password + "';");

    pool.releaseConnection(conn);

    return response && response->status == 200;
}

bool DBU::registerUser(const std::string &username, const std::string &password, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "INSERT INTO MCrossUDB (username, password) VALUES ('" + username + "', '" + password + "');");

    pool.releaseConnection(conn);

    return response && response->status == 200;
}

std::string DBU::escape_sql(const std::string &str)
{
    return "\"" + str + "\"";
}


bool DBU_Worker::insertMessage(const ChatMessage &message, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "INSERT INTO MCrossMDB (sender, receiver, message, timestamp) VALUES ("+ escape_sql(message.sender) + "," + escape_sql(message.receiver) + "," + escape_sql(message.message) + ", now());");
    pool.releaseConnection(conn);

    return response && response->status == 200;
}

bool DBU_Worker::exisitsUser(const std::string &username, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "SELECT * FROM MCrossUDB WHERE username = " + escape_sql(username) + ";");

    pool.releaseConnection(conn);

    return response && response->status == 200;
}

bool DBU_Worker::checkUser(const std::string &username, const std::string &password, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "SELECT * FROM MCrossUDB WHERE username = '" + username + "' AND password = '" + password + "';");

    pool.releaseConnection(conn);

    return response && response->status == 200;
}

bool DBU_Worker::registerUser(const std::string &username, const std::string &password, ConnectPool &pool)
{
    auto conn = pool.getConnection();
    auto response = conn.Post(
        "INSERT INTO MCrossUDB (username, password) VALUES ('" + username + "', '" + password + "');");

    pool.releaseConnection(conn);

    return response && response->status == 200;
}

std::string DBU_Worker::escape_sql(const std::string &str)
{
    return "\"" + str + "\"";
}