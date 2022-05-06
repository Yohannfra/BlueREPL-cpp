#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

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
    bool is_mac_address(const std::string &str);

    /**
     * @brief Check if a string is a positive number
     *
     * @param str string to check
     * The number must look like a normal integer : 0, 55, 39326 ...
     *
     * @return true or false
     */
    bool is_number(const std::string &str);
} // namespace Utils

#endif
