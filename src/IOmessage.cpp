#include "IOmessage.h"

bool IOMessage::is_empty_I()
{
    std::shared_lock<std::shared_mutex> lock(Imutex_);
    return Iqueue_.empty();
}


bool IOMessage::is_empty_O()
{
    std::shared_lock<std::shared_mutex> lock(Omutex_);
    return Oqueue_.empty();
}

void IOMessage::write_o(std::string& message)
{
    std::unique_lock<std::shared_mutex> lock(Omutex_);
    Oqueue_.push(message);
    Ocond_.notify_one();
}

void IOMessage::write_i(std::string& message)
{
    std::unique_lock<std::shared_mutex> lock(Imutex_);
    Iqueue_.push(message);
    Icond_.notify_one();
}

std::string IOMessage::read_o()
{
    std::shared_lock<std::shared_mutex> lock(Omutex_);
    Ocond_.wait(lock, [this] { return !Oqueue_.empty(); });
    std::string message = Oqueue_.front();
    Oqueue_.pop();
    return message;
}

std::string IOMessage::read_i()
{
    std::unique_lock<std::shared_mutex> lock(Imutex_);
    Icond_.wait(lock, [this] { return !Iqueue_.empty(); });
    std::string message = Iqueue_.front();
    Iqueue_.pop();
    return message;
}

std::shared_mutex& IOMessage::in_lock()
{
    return Imutex_;
}

std::shared_mutex& IOMessage::out_lock()
{
    return Omutex_;
}

std::condition_variable_any& IOMessage::in_cond()
{
    return Icond_;
}

std::condition_variable_any& IOMessage::out_cond()
{
    return Ocond_;
}