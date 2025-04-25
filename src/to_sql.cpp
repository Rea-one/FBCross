#include "to_sql.h"

std::string reference(const std::string& message)
{
    return std::move("'" + message + "'");
}