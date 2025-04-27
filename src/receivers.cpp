#include "receivers.h"


frm_handler::frm_handler(std::shared_ptr<IOMessage> io_mes):
    io_mes_(io_mes)
{}

void frm_handler::get_message(boost::asio::ip::tcp::socket& socket)
{
    std::string ip = socket.remote_endpoint().address().to_string();
    std::string message;
    try
    {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');
        std::istream is(&buffer);
        std::getline(is, message);
    }
    catch(std::exception& e)
    {
        
    }
    io_mes_ -> write_o(message);
}



void frm_handler::put_message(boost::asio::ip::tcp::socket& socket)
{
    std::string ip = socket.remote_endpoint().address().to_string();
    std::string message;

    message = io_mes_ -> read_o();

    if (message != "")
    {
        boost::asio::write(socket, boost::asio::buffer(message));
    }
}

using io_type = std::unordered_map<std::string, std::shared_ptr<IOMessage>>;
Receivers::Receivers(const int &receiver_limit, std::shared_ptr<LimPool> pool, std::shared_ptr<io_type> io_mes):
    passer_limit_(receiver_limit), pool_(pool), io_mes_(io_mes)
{}

void Receivers::start()
{
    while (true)
    {
        if (current_passer_ < passer_limit_ && !pool_ -> is_empty())
        {
            auto socket = pool_ -> get();
            auto target = std::make_shared<MessPasser>();
            auto ip = socket.remote_endpoint().address().to_string();
            target -> link(std::move(std::make_shared<boost::asio::ip::tcp::socket>(socket)));
            target -> mlink(std::move(io_mes_ -> find(ip) -> second));
            target -> start();
            passers_.push(target);
            current_passer_++;
        }
    }
}