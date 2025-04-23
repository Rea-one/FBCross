#include "receiver.h"


frm_handler::frm_handler(std::shared_ptr<IOMesaage> io_mes):
    io_mes_(io_mes)
{}

std::string frm_handler::get_message(boost::asio::ip::tcp::socket& socket)
{
    std::string ip;
    auto message = io_mes_ -> read_o(ip);
    if (!message.empty())
    {
        
    }
}

std::string frm_handler::operation(std::string message)
{
    
}

void frm_handler::put_message(std::string& message, boost::asio::ip::tcp::socket& socket)
{
    
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
            std::string message;
            while(true)
            {
                message = handler.get_message(socket);
                message = handler.operation(message);
                handler.put_message(message, socket);
            }
        });
    }
}