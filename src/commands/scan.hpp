#ifndef SCAN_HPP
#define SCAN_HPP

#include "BleController.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Scan {
    public:
        static void print_help()
        {
            std::cout << "help\n";
            std::cout << "\nDescription:\n\tscan ble devices arround\n";
            std::cout << "\nParameters:\n\ttimeout: scan time (in second)" << std::endl;
        }

        static int run(BleController &bt, std::vector<std::string> &args)
        {
            if (args.size() > 1) {
                std::cerr << "Invalid usage, see 'help scan'" << std::endl;
                return EXIT_FAILURE;
            } else if (args.size() == 1) {
                int timeout;
                try {
                    timeout = std::stoi(args.at(0));
                } catch (std::invalid_argument) {
                    std::cout << "'" << args.at(0) << "' Not a number" << std::endl;
                }
                bt.scan(timeout);
            } else {
                bt.scan();
            }
            return 0;
        }
    };
} // namespace Command

#endif
