#include "utils.hpp"

#include <regex>
#include <string>

bool Utils::is_mac_address(const std::string &str)
{
    constexpr char REGEX_STR[] = "[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}";

    return std::regex_match(str, std::regex(REGEX_STR));
}

bool Utils::is_number(const std::string &str)
{
    constexpr char REGEX_STR[] = "^[0-9]{1,}$";

    return std::regex_match(str, std::regex(REGEX_STR));
}
