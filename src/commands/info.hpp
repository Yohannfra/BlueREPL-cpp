#ifndef INFO_HPP
#define INFO_HPP

#include "BleController.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Info {
    public:
        static void print_help()
        {
            std::cout << "help\n";
            std::cout << "\nDescription:\n\tinfo print informations about 'something'\n";
            std::cout << "\nParameters:\n\t'something': print informations about this\n";
            std::cout << "\tcan be any of the following:\n";
            std::cout << "\t\t- device: ble device connected\n";
            std::cout << "\t\t- services: services of the ble device connected" << std::endl;
        }

        static int run(BleController &bt, std::vector<std::string> &args)
        {
            if (args.empty()) {
                std::cerr << "Invalid usage, see 'help info'" << std::endl;
                return EXIT_FAILURE;
            }
            for (auto &arg : args) {
                if (arg == "device") {
                    return bt.print_peripheral_infos();
                } else if (arg == "services") {
                    return bt.print_peripheral_services();
                }
            }
            return EXIT_FAILURE;
        }
    };
} // namespace Command

#endif
