#include "workers.h"

// bkm_handler::bkm_handler(std::shared_ptr<IOMessage> io_mes) : io_mes_(io_mes)
// {
// }

// std::string bkm_handler::get_message()
// {
//     auto [ip, message] = io_mes_->read_i();
//     user_ip_ = ip;
//     return message;
// }

// std::string bkm_handler::operation(std::string message, pqxx::connection &conn)
// {
//     std::string sql_mess{};
//     std::string now{};
//     std::unique_ptr<std::string> cursor{};
//     for (auto &elem : message)
//     {
//         if (elem == ' ')
//         {
//             if (cursor != nullptr)
//             {
//                 *cursor = now;
//                 now.clear();
//                 cursor.reset();
//                 continue;
//             }
//             if (now == "insert")
//             {
//                 task = now;
//                 now.clear();
//             }
//             else if (now == "sender")
//             {
//                 cursor = std::make_unique<std::string>(sender);
//             }
//             else if (now == "receiver")
//             {
//                 cursor = std::make_unique<std::string>(receiver);
//             }
//             else if (now == "message")
//             {
//                 cursor = std::make_unique<std::string>(pmessage);
//             }
//         }
//         else
//         {
//             now += elem;
//         }
//     }

//     if (task == "insert")
//     {
//         sql_mess = "insert into messages (sender, receiver, message) values (" + reference(sender) + "," + reference(receiver) + "," + reference(pmessage) + ");";
//     }

//     try
//     {
//         pqxx::work txn(conn);
//         txn.exec(sql_mess);
//         txn.commit();
//     }
//     catch (const std::exception &e)
//     {
//         // 处理错误（如回滚事务）
//         return "Error: " + std::string(e.what());
//     }
//     return "Success: Inserted into database.";
// }

// void bkm_handler::put_message(std::string &message)
// {
//     std::unique_lock<std::mutex> lock(io_mes_->Imutex_);
//     io_mes_->write_o(user_ip_, message);
// }

using io_type = std::unordered_map<std::string, std::shared_ptr<IOMessage>>;

Workers::Workers(const int &worker_limit, std::shared_ptr<PQPool> pool, std::shared_ptr<io_type> io_mes):
    worker_limit_(worker_limit), current_workers_(0),
    pool_(std::move(pool)), io_mes_(std::make_shared<io_type>(io_mes))
{
}

void Workers::start()
{
    while (true) 
    {
        while (current_workers_ >= worker_limit_)
        {
            std::cout << "Waiting for workers to finish..." << std::endl;
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        }
        if (current_workers_ < worker_limit_)
        {
            std::unique_lock<std::mutex> lock(pool_->pool_lock());
            pool_->pool_cv().wait(lock, [this]() { return !pool_->is_empty(); });
            try
            {
                auto conn = std::move(pool_->get());
                auto target = std::make_shared<Worker>();
                target -> link(std::make_unique<pqxx::connection> (conn));
                target -> start();
                workers_.push(target);
                current_workers_ ++;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                break;
            }
        }
        while (!workers_.empty())
        {
            try
            {
                auto target = io_mes_->begin();
                if (target != io_mes_->end())
                {
                    workers_.top() -> mlink(std::move(target->second));
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                break;
            }
        }
        
        
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }
}

