#include "workers.h"


bkm_handler::bkm_handler(std::shared_ptr<IOMesaage> io_mes):
    io_mes_(io_mes)
{}

std::string bkm_handler::get_message()
{
    auto [ip, message] = io_mes_ -> read_i();
    user_ip_ = ip;
    return message;
}

std::string bkm_handler::operation(std::string message, boost::asio::ip::tcp::socket& socket)
{
    
}

void bkm_handler::put_message(std::string& message)
{
    std::unique_lock<std::mutex> lock(io_mes_ -> Imutex_);
    io_mes_ -> write_o(user_ip_, message);
}


Workers::Workers(const int &worker_limit, ExpPool& pool, IOMesaage& io_mes):
    worker_limit_(worker_limit), current_workers_(0)
{
    pool_ = std::make_shared<ExpPool>(pool);
    io_mes_ = std::make_shared<IOMesaage>(io_mes);
}

void Workers::start()
{
    while(!pool_ -> is_empty())
    {
        auto socket = std::move(pool_ -> get());
        workers_.submit
        ([this, &socket] ()
        {
            bkm_handler handler(io_mes_);
            std::string message;
            while(true)
            {
                message = handler.get_message();
                message = handler.operation(message, socket);
                handler.put_message(message);
            }
        });
    }
}

void Workers::wait()
{
    workers_.wait();
}
