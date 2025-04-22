#include <string>

std::string reference(const std::string& message)
{
    return std::move("\"" + message + "\"");
}


