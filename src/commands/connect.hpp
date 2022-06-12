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

            _args.add({"identifier", "identifier of the device to connect to", true,
                false, false});

            _help = "The identifier can be of three different types:\n"
                    "\t- integer -> connection by index of the device in the scan list\n"
                    "\t- a string mac address (format "
                    "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX) -> connection by "
                    "using the mac address of the device\n"
                    "\t- any other string -> connection by using name of the device";
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            if (args.size() != 1) {
                std::cerr << "Invalid usage, see 'help connect'" << std::endl;
                return EXIT_FAILURE;
            }
            return connectById(args.at(0), bt);
        }

        int connectById(const std::string &deviceid, BleController &bt)
        {
            int ret;

            if (Utils::isNumber(deviceid)) {
                std::cout << "Connecting by index" << std::endl;
                ret = bt.connectByIndex(std::stoi(deviceid));
            } else if (Utils::isMacAddress(deviceid)) {
                std::cout << "Connecting by address" << std::endl;
                ret = bt.connectByAddress(deviceid);
            } else {
                std::cout << "Connecting by name" << std::endl;
                ret = bt.connectByName(deviceid);
            }
            return ret;
        }
    };
} // namespace Command

#endif
