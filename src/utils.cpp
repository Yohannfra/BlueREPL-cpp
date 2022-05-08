#include "utils.hpp"

#include <fmt/core.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <string>

bool Utils::isMacAddress(const std::string &str)
{
    constexpr char REGEX_STR[] = "[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}";

    return std::regex_match(str, std::regex(REGEX_STR));
}

bool Utils::isNumber(const std::string &str)
{
    constexpr char REGEX_STR[] = "^[0-9]{1,}$";

    return std::regex_match(str, std::regex(REGEX_STR));
}

bool Utils::isAsciiWord(const std::string &str)
{
    const auto isLetter = [](char c) {
        return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z')); //
    };

    if (std::any_of(str.begin(), str.end(), isLetter)) {
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
    for (const auto &c : s) {
        if (!((c >= '0' && c <= '9') || c == '-')) {
            return false;
        }
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
