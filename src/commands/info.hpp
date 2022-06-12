#ifndef INFO_HPP
#define INFO_HPP

#include "BleController.hpp"
#include "ICommand.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Info : public ICommand {
    public:
        Info()
        {
            _name = "info";
            _hint = "print informations about topic";
            _usage = "device | services";

            _args.add({"device", "\tPrint information about the ble device connected",
                false, false, true});
            _args.add({"services",
                "Describes the services and characteristics of the ble device connected",
                false, false, true});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            if (args.size() != 1) {
                std::cerr << "Invalid usage, see 'help info'" << std::endl;
                return EXIT_FAILURE;
            }

            const std::string topic = args.at(0);

            if (topic == "device") {
                return bt.print_peripheral_infos();
            } else if (topic == "services") {
                return bt.print_peripheral_services();
            } else {
                std::cerr << "Unknown topic '" << topic << "'" << std::endl;
            }
            return EXIT_FAILURE;
        }
    };
} // namespace Command

#endif
