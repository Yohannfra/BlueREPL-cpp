#ifndef SCAN_HPP
#define SCAN_HPP

#include "BleController.hpp"
#include "ICommand.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Scan : public ICommand {
    public:
        Scan()
        {
            _name = "scan";
            _hint = "Search for BLE devices around";
            _usage = "[timeout] [filter]"; // TODO implement filter
            _args.add({"timeout", "scan duration (in seconds), 5 by default", false,
                false, false});
            _args.add({"filter", "filter name", false, false, false});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            if (args.size() > _args.getNumberArgs()) {
                std::cerr << "Invalid usage, see 'help scan'" << std::endl;
                return EXIT_FAILURE;
            } else if (args.size() == 1) {
                int timeout;
                try {
                    timeout = std::stoi(args.at(0));
                } catch (std::invalid_argument) {
                    std::cerr << "'" << args.at(0) << "' Not a number" << std::endl;
                    return EXIT_FAILURE;
                }
                bt.scan(timeout);
            } else {
                bt.scan();
            }
            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
