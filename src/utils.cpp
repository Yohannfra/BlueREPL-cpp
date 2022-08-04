#include "utils.hpp"

#include <fmt/core.h>
#include <iomanip>
#include <limits>
#include <regex>
#include <sstream>

bool Utils::isMacAddress(const std::string &str)
{
    constexpr char REGEX_STR[] =
        "[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}";

    return std::regex_match(str, std::regex(REGEX_STR));
}

bool Utils::isNumber(const std::string &str)
{
    constexpr char REGEX_STR[] = "^[0-9]{1,}$";

    return std::regex_match(str, std::regex(REGEX_STR));
}

bool Utils::isAsciiWord(const std::string &str)
{
    const auto isntLetter = [](char c) {
        return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c == '_')); //
    };

    if (std::any_of(str.begin(), str.end(), isntLetter)) {
        return false;
    }
    return true;
}

void Utils::printAsAscii(const std::string &str)
{
    std::cout << str << std::endl;
}

void Utils::printAsHex(const std::string &str)
{
    for (auto &c : str) {
        std::cout << fmt::format("{:#02x}", (int)c) << std::endl;
    }
}

void Utils::printAsDec(const std::string &str)
{
    for (auto &c : str) {
        std::cout << (int)c << std::endl;
    }
}

void Utils::printAsBinary(const std::string &str)
{
    for (auto &c : str) {
        auto val = fmt::format("{:#b}", (int)c);
        while (val.size() < 8) {
            val = "0" + val;
        }
        std::cout << val << std::endl;
    }
}

void Utils::printBleValue(const std::string &str, BleDataType type)
{
    switch (type) {
        case BleDataType::HEXADECIMAL:
            Utils::printAsHex(str);
            break;
        case BleDataType::DECIMAL:
            Utils::printAsDec(str);
            break;
        case BleDataType::ASCII:
            std::cout << str << std::endl;
            break;
        case BleDataType::BINARY:
            Utils::printAsBinary(str);
            break;
        default:
            break;
    }
}

std::vector<std::string> Utils::splitInArgs(const std::string &command)
{
    std::stringstream ss(command);
    std::string word;
    std::vector<std::string> cmds;

    while (ss >> std::quoted(word)) {
        cmds.push_back(word);
    }
    return cmds;
}

std::optional<ByteArray> Utils::parseHexStr(const std::string &str)
{
    ByteArray out;

    auto splitted = Utils::splitInArgs(str);

    for (const auto &b : splitted) {
        long n;

        try {
            n = std::stol(b, nullptr, 16);
        } catch (std::invalid_argument) {
            std::cerr << "Invalid hex value '" << b << "'" << std::endl;
            return {};
        }

        if (n > std::numeric_limits<unsigned char>::max()) {
            std::cerr << "'" << b << "' is larger than 0xFF" << std::endl;
            return {};
        }

        out.push_back(n);
    }

    return out;
}

bool Utils::isDecimalNumber(const std::string &s)
{
    const auto isntdec = [](char c) {
        return !((c >= '0' && c <= '9') || c == '-');
    };

    if (std::any_of(s.begin(), s.end(), isntdec) || s.empty()) {
        return false;
    }

    return true;
}

bool Utils::isBinaryNumber(const std::string &s)
{
    const auto isntbin = [](char c) {
        return (c != '0' && c != '1');
    };

    if (std::any_of(s.begin(), s.end(), isntbin) || s.empty()) {
        return false;
    }
    return true;
}

std::optional<ByteArray> Utils::parseDecStr(const std::string &str)
{
    ByteArray out;

    auto splitted = Utils::splitInArgs(str);

    for (const auto &b : splitted) {
        long n;

        if (!Utils::isDecimalNumber(b)) {
            std::cerr << "Not a number '" << b << "'" << std::endl;
            return {};
        }

        try {
            n = std::stol(b, nullptr, 10);
        } catch (std::invalid_argument) {
            std::cerr << "Invalid decimal value '" << b << "'" << std::endl;
            return {};
        }

        if (n > std::numeric_limits<unsigned char>::max()) {
            std::cerr << "'" << b << "' is larger than 255" << std::endl;
            return {};
        }

        if (n < 0) {
            std::cerr << "Negative values not supported yet '" << b << "'" << std::endl;
            return {};
        }

        out.push_back(n);
    }

    return out;
}

std::optional<ByteArray> Utils::parseBinStr(const std::string &str)
{
    ByteArray out;

    auto splitted = Utils::splitInArgs(str);

    for (const auto &b : splitted) {
        long n;

        if (!Utils::isBinaryNumber(b)) {
            return {};
        }

        try {
            n = std::stol(b, nullptr, 2);
        } catch (std::invalid_argument) {
            std::cerr << "Invalid binary value '" << b << "'" << std::endl;
            return {};
        }

        if (n > std::numeric_limits<unsigned char>::max()) {
            std::cerr << "'" << b << "' is larger than 11111111 (0xff)" << std::endl;
            return {};
        }

        out.push_back(n);
    }

    return out;
}

void Utils::printStringVector(const std::vector<std::string> &vec)
{
    std::cout << "{ ";
    for (const auto &v : vec) {
        std::cout << "'" << v << "' ";
    }
    std::cout << "}" << std::endl;
}

std::map<std::string, BleDataType> Utils::tokenizeLine(const std::string &line)
{
    std::map<std::string, BleDataType> mp;

    auto spl = Utils::splitInArgs(line);

    for (auto &w : spl) {
        if (w.at(0) == '\'' && w.at(w.size() - 1) == '\'') {
            mp[w] = BleDataType::ASCII;
        } else if (w.rfind("0x", 0) == 0 || w.rfind("0X", 0) == 0) {
            mp[w] = BleDataType::HEXADECIMAL;
        } else if (w.rfind("0b", 0) == 0 || w.rfind("0B", 0) == 0) {
            mp[w] = BleDataType::BINARY;
        } else if (Utils::isDecimalNumber(w)) {
            mp[w] = BleDataType::DECIMAL;
        } else {
            std::cerr << "Unknown argument type: '" << w << "'" << std::endl;
            return {};
        }
    }
    return mp;
}
