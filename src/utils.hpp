#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#define BOLD(s)  "\x1b[1m" s "\x1b[0m"
#define RED(s)   "\x1b[0;31m" s "\x1b[0m"
#define GREEN(s) "\x1b[0;32m" s "\x1b[0m"

enum class BleDataType { ASCII, BINARY, DECIMAL, HEXADECIMAL };

using BluetoothUUID = std::string;
using BluetoothAddress = std::string;
using ByteArray = std::string;

namespace Utils {
    /**
     * @brief Check if a string has the format of a mac address
     *
     * @param str string to check
     *
     * The mac address must look to something like this:
     * FC32B125-4E7F-DD16-A05B-9E8025D2FA28
     * fc32b125-4e7f-dd16-a05b-9e8025d2fa28
     * XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
     * ...
     *
     * @return true or false
     */
    bool isMacAddress(const std::string &str);

    /**
     * @brief Check if a string is a positive number
     *
     * @param str string to check
     * The number must look like a normal integer : 0, 55, 39326 ...
     *
     * @return true or false
     */
    bool isNumber(const std::string &str);

    void printAsAscii(const std::string &str);
    void printAsHex(const std::string &str);
    void printAsDec(const std::string &str);
    void printAsBinary(const std::string &str);
    void printBleValue(const std::string &str, BleDataType type);

    std::optional<ByteArray> parseHexStr(const std::string &str);
    std::optional<ByteArray> parseDecStr(const std::string &str);
    std::optional<ByteArray> parseBinStr(const std::string &str);

    /**
     * @brief Split a string in a list of argument, take quotes into account
     *
     * Adapted from here: https://stackoverflow.com/a/69809028/11025595
     *
     * @param command input string
     *
     * @return vector of argument
     */
    std::vector<std::string> splitInArgs(const std::string &command);

    /**
     * @brief Tells if a string represents a decimal number
     *
     * ex: 123 12 3 0 ...
     *
     * @param str string to check
     *
     * @return true or false
     */
    bool isDecimalNumber(const std::string &str);

    /**
     * @brief Tells if a string is only composed of ascii letters
     * [a-z][A-Z]
     *
     * @param str string to check
     *
     * @return true or false
     */
    bool isAsciiWord(const std::string &str);

    /**
     * @brief Pretty print a vector of strings
     *
     * @param vec vec to print
     */
    void printStringVector(const std::vector<std::string> &vec);
} // namespace Utils

#endif
