#ifndef DISCONNECT_HPP
#define DISCONNECT_HPP

#include "BleController.hpp"
#include "ICommand.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Disconnect : public ICommand {
    public:
        Disconnect()
        {
            _name = "disconnect";
            _hint = "disconnect from the ble device";
        }

        int run(std::vector<std::string> &args, BleController &bt) override
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
