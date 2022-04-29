#ifndef DISCONNECT_HPP
#define DISCONNECT_HPP

#include "BleController.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Disconnect {
    public:
        static void print_help()
        {
            std::cout << "help" << '\n';
            std::cout << "\nDescription:\n\tdisconnect from the ble device" << std::endl;
        }

        static int run(BleController &bt, std::vector<std::string> &args)
        {
            if (!args.empty()) {
                std::cerr << "Invalid usage, see 'help disconnect'" << std::endl;
                return EXIT_FAILURE;
            }
            return bt.disconnect();
        }
    };
} // namespace Command

#endif
