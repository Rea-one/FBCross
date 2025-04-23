#include "receiver.h"


frm_handler::frm_handler(std::shared_ptr<IOMesaage> io_mes):
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
    io_mes_ -> write_o(ip, message);
}



void frm_handler::put_message(boost::asio::ip::tcp::socket& socket)
{
    std::string ip = socket.remote_endpoint().address().to_string();
    std::string message;

    message = io_mes_ -> read_o(ip);

    if (message != "")
    {
        boost::asio::write(socket, boost::asio::buffer(message));
    }
}


Receiver::Receiver(const int &receiver_limit, CONNECT_POOL& pool, IOMesaage& io_mes):
    receiver_limit_(receiver_limit), queues_(receiver_limit_)
{
    pool_ = std::make_shared<LimPool>(io_mes);
    io_mes_ = std::make_shared<IOMesaage>(io_mes);
}

void Receiver::start()
{
    while(!pool_ -> is_empty())
    {
        auto socket = std::move(pool_ -> get());
        receivers_.submit
        ([this, &socket] ()
        {
            frm_handler handler(io_mes_);
            while(true)
            {
                handler.get_message(socket);
                handler.put_message(socket);
            }
        });
    }
}