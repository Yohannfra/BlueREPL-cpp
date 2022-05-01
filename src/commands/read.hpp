#ifndef READ_HPP
#define READ_HPP

#include "BleController.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Read {
    public:
        static void print_help()
        {
            std::cout << "help" << '\n';
            std::cout << "\nDescription:\n\tread a characteristic value" << std::endl;
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
            if (args.size() != 2) {
                std::cerr << "Invalid usage, see 'help connect'" << std::endl;
                return EXIT_FAILURE;
            }

            BluetoothUUID service = args.at(0);
            BluetoothUUID characteristic = args.at(1);
            auto vals = bt.read(service, characteristic);
            for (size_t i = 0; i < vals.size(); i++) {
                std::cout << (int)vals.at(i) << std::endl;
            }

            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
