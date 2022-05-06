#ifndef CONNECT_HPP
#define CONNECT_HPP

#include "BleController.hpp"
#include "ICommand.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Connect : public ICommand {
    public:
        Connect()
        {
            _name = "connect";
            _hint = "Connect to a ble device";
            _usage = "identifier";

            _args.add({"identifier", "identifier of the device to connect to", true, false, false});

            _help = "The identifier can be of three different types:\n"
                    "\t- integer -> connection by index of the device in the scan list\n"
                    "\t- a string mac address (format XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX) -> connection by "
                    "using the mac address of the device\n"
                    "\t- any other string -> connection by using name of the device";
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            if (args.size() != 1) {
                std::cerr << "Invalid usage, see 'help connect'" << std::endl;
                return EXIT_FAILURE;
            }

            if (Utils::isNumber(args.at(0))) {
                std::cout << "Connecting by index" << std::endl;
                bt.connectByIndex(std::stoi(args.at(0)));
            } else if (Utils::isMacAddress(args.at(0))) {
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
