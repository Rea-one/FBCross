#include "mess_passer.h"

void MessPasser::start()
{
    thread_ = boost::thread([this]()
    {
        while (!stop_)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            active_ = true;
            if (!socket_)
                cv_.wait(lock);
            if (socket_ && io_message_)
            {
                if (io_message_->is_empty_I())
                    cv_.wait(lock);
                else
                {
                    input();
                    output();
                }

            }
        }
    });
}

void MessPasser::stop()
{
    active_ = false;
    stop_ = true;
    cv_.notify_one();
    thread_.join();
}

bool MessPasser::link(std::shared_ptr<boost::asio::ip::tcp::socket>&& socket)
{
    if (socket_)
        return false;
    else
    {
        std::unique_lock<std::mutex> lock(mutex_);
        socket_ = std::move(socket);
        cv_.notify_one();
        return true;
    }
}

bool MessPasser::mlink(std::shared_ptr<IOMessage>&& io_message)
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

void MessPasser::input()
{
    std::string message;
    boost::asio::streambuf buffer;
    boost::asio::read_until(*socket_, buffer, '\n');
    std::istream is(&buffer);
    std::getline(is, message);
    if (message != "")
    {
        io_message_->write_i(message);
    }
}

void MessPasser::output()
{
    if (!io_message_->is_empty_O())
    {
        boost::asio::write(*socket_, boost::asio::buffer(io_message_->read_o()));
    }
}
