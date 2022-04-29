#ifndef CONNECT_HPP
#define CONNECT_HPP

#include "BleController.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Connect {
    public:
        static void print_help()
        {
            std::cout << "help" << '\n';
            std::cout << "\nDescription:\n\tconnect to a ble device" << std::endl;
            std::cout << "\nParameters:\n\tdevice_identifier: identifier of the device (found in "
                         "scan command)\n";
            std::cout << "\tThe identifier can be of three different type:\n";
            std::cout << "\t- integer -> connection by index of the device in the scan list" << std::endl;
            std::cout << "\t- a string mac address (format XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX)  -> connection "
                         "by using the mac address of the device\n";
            std::cout << "\t- any other string -> connection by using name of the device" << std::endl;
        }

        static int run(BleController &bt, std::vector<std::string> &args)
        {
            if (args.size() != 1) {
                std::cerr << "Invalid usage, see 'help connect'" << std::endl;
                return EXIT_FAILURE;
            }

            if (Utils::is_number(args.at(0))) {
                std::cout << "Connecting by index" << std::endl;
                bt.connectByIndex(std::stoi(args.at(0)));
            } else if (Utils::is_mac_address(args.at(0))) {
                std::cout << "Connecting by address" << std::endl;
                bt.connectByAddress(args.at(0));
            } else {
                std::cout << "Connecting by name" << std::endl;
                bt.connectByName(args.at(0));
            }

            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
