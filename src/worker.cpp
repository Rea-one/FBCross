#include "worker.h"

void Worker::start()
{
    thread_ = boost::thread([this]()
    {
        while (!stop_)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            active_ = true;
            if (!conn_)
                cv_.wait(lock);
            if (conn_ && io_message_)
            {
                if (io_message_->is_empty_I())
                    cv_.wait(lock);
                else
                {
                    std::string back = operation(io_message_->read_i());
                    io_message_->write_o(back);
                }

            }
        }
    });
}

void Worker::stop()
{
    active_ = false;
    stop_ = true;
    cv_.notify_one();
    thread_.join();
}

bool Worker::link(std::unique_ptr<pqxx::connection>&& conn)
{
    if (conn_)
        return false;
    else
    {
        std::unique_lock<std::mutex> lock(mutex_);
        conn_ = std::move(conn);
        cv_.notify_one();
        return true;
    }
}

bool Worker::mlink(std::shared_ptr<IOMessage>&& io_message)
{
    if (io_message_)
        return false;
    else
    {
        std::unique_lock<std::mutex> lock(mutex_);
        io_message_ = std::move(io_message);
        cv_.notify_one();
        return true;
    }
}

bool Worker::submit(std::function<std::string()>&& task)
{
    if (!active_)
        return false;
    else
    {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
        cv_.notify_one();
        return true;
    }
}

std::string Worker::operation(std::string message)
{
    std::string task{};
    std::string sql_mess{};
    std::string now{};
    std::unique_ptr<std::string> cursor{};
    for (auto &elem : message)
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
                cursor = std::make_unique<std::string>(sender_);
            }
            else if (now == "receiver")
            {
                cursor = std::make_unique<std::string>(receiver_);
            }
            else if (now == "message")
            {
                cursor = std::make_unique<std::string>(message_);
            }
        }
        else
        {
            now += elem;
        }
    }

    if (task == "insert")
    {
        sql_mess = "insert into messages (sender, receiver, message) values (" +
            reference(sender_) + "," +
            reference(receiver_) + "," +
            reference(message_) + ");";
    }

    try
    {
        pqxx::work txn(*conn_);
        txn.exec(sql_mess);
        txn.commit();
    }
    catch (const std::exception &e)
    {
        // 处理错误（如回滚事务）
        return "Error: " + std::string(e.what());
    }
    return "Success: Inserted into database.";
}
