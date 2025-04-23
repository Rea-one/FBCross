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

std::string bkm_handler::operation(std::string message, pqxx::connection& conn)
{
    std::string sql_mess{};
    std::string now{};
    std::unique_ptr<std::string> cursor{};
    for (auto& elem: message)
    {
        if (elem == ' ')
        {
            if (cursor != nullptr)
            {
                *cursor = now;
                now.clear();
                cursor.reset();
                continue;
            }
            if (now == "insert")
            {
                task = now;
                now.clear();
            }
            else if (now == "sender")
            {
                cursor = std::make_unique<std::string> (sender);
            }
            else if (now == "receiver")
            {
                cursor = std::make_unique<std::string> (receiver);
            }
            else if (now == "message")
            {
                cursor = std::make_unique<std::string> (pmessage);
            }
        }
        else
        {
            now += elem;
        }
    }

    if (task == "insert")
    {
        sql_mess = "insert into messages (sender, receiver, message) values ("
            + reference(sender) + ","
            + reference(receiver) + ","
            + reference(pmessage) + ");";
    }

    pqxx::work txn(conn); // 创建事务
    txn.exec(sql_mess);   // 执行SQL
    txn.commit();         // 提交事务
    return "Success: Inserted into database.";
}

void bkm_handler::put_message(std::string& message)
{
    std::unique_lock<std::mutex> lock(io_mes_ -> Imutex_);
    io_mes_ -> write_o(user_ip_, message);
}


Workers::Workers(const int &worker_limit, PQPool& pool, IOMesaage& io_mes):
    worker_limit_(worker_limit), current_workers_(0)
{
    pool_ = std::make_shared<PQPool>(pool);
    io_mes_ = std::make_shared<IOMesaage>(io_mes);
}

void Workers::start()
{
    while(!pool_ -> is_empty())
    {
        auto conn = std::move(pool_ -> get());
        workers_.submit
        ([this, &conn] ()
        {
            bkm_handler handler(io_mes_);
            std::string message;
            while(true)
            {
                message = handler.get_message();
                message = handler.operation(message, conn);
                handler.put_message(message);
            }
        });
    }
}

void Workers::wait()
{
    workers_.wait();
}
