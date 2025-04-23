#include "listener.h"


Listener::Listener(const std::string& sever_ip, int sever_port):
    acceptor_(io_context_,
        boost::asio::ip::tcp::endpoint{
            boost::asio::ip::make_address(sever_ip),
            static_cast<unsigned short>(sever_port)
        })
{

}

void Listener::start()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket sock)
        {
            handle_accept(std::move(sock));
            start();
        }
    );
}

void Listener::handle_accept(boost::asio::ip::tcp::socket&& socket)
{
    pool_ -> submit(std::move(socket));
}


void Listener::link_pool(std::shared_ptr<LimPool>&& pool)
{
    pool_ = std::move(pool);
}