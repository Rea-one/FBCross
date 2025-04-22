#include <string>


#include "boost/asio.hpp"

#include "threads.h"
#include "connect_pool.h"

class Listener
{
private:
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::unique_ptr<ConnectPool> pool_;
public:
    Listener(const std::string& sever_ip, int sever_port);
    void start();
    void handle_accept(boost::asio::ip::tcp::socket&& socket);
};