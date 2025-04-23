#include "IOMessage.h"

bool IOMessage::is_empty_I()
{
    std::lock_guard<std::mutex> lock(Imutex_);
    return Iqueue_.empty();
}


bool IOMessage::is_empty_O()
{
    std::lock_guard<std::mutex> lock(Omutex_);
    return Oqueue_.empty();
}

void IOMessage::write_o(std::string& ip, std::string& message)
{
    std::lock_guard<std::mutex> lock(Omutex_);
    Oqueue_[ip].push(message);
    Ocond_.notify_one();
}

void IOMessage::write_i(std::string& ip, std::string& message)
{
    std::lock_guard<std::mutex> lock(Imutex_);
    Iqueue_.push({ip, message});
    Icond_.notify_one();
}

std::string IOMessage::read_o(std::string& ip)
{
    std::unique_lock<std::mutex> lock(Omutex_);
    Ocond_.wait(lock, [this, &ip] { return !Oqueue_[ip].empty(); });
    std::string message = Oqueue_[ip].front();
    Oqueue_[ip].pop();
    return message;
}

std::pair<std::string, std::string> IOMessage::read_i()
{
    std::unique_lock<std::mutex> lock(Imutex_);
    Icond_.wait(lock, [this] { return !Iqueue_.empty(); });
    std::pair<std::string, std::string> message = Iqueue_.front();
    Iqueue_.pop();
    return message;
}