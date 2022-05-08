#ifndef UNSUBSCRIBE_HPP
#define UNSUBSCRIBE_HPP

#include "BleController.hpp"
#include "ICommand.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Unsubscribe : public ICommand {
    public:
        Unsubscribe()
        {
            _name = "unsubscribe";
            _hint = "Unsubscribe from a characteristic notifications";
            _usage = "service_uuid characteristic_uuid";

            _args.add(
                {"service_uuid", "\tuuid of the service in which the characteristic is", true, false, false});
            _args.add(
                {"characteristic_uuid", "uuid of the characteristic to unsubscribe from", true, false, false});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            if (args.size() != _args.getNumberOfRequiredArgs()) {
                std::cerr << "Invalid usage, see 'help read'" << std::endl;
                return EXIT_FAILURE;
            }

            BluetoothUUID service = args.at(0);
            BluetoothUUID characteristic = args.at(1);
            return bt.unsubscribe(service, characteristic);
        }
    };
} // namespace Command

#endif
