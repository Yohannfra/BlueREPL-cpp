#ifndef READ_HPP
#define READ_HPP

#include "BleController.hpp"
#include "ICommand.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Read : public ICommand {
    public:
        Read()
        {
            _name = "read";
            _hint = "Read a characteristic value and print it";
            _usage = "service_uuid characteristic_uuid [-a] [-x] [-d]";

            _args.add(
                {"service_uuid", "\tuuid of the service in which the characteristic is",
                    true, false, false});
            _args.add({"characteristic_uuid", "uuid of the characteristic to read", true,
                false, false});
            _args.add({"-a", "\t\tprint as ascii", false, true, true});
            _args.add({"-b", "\t\tprint as binary", false, true, true});
            _args.add({"-d", "\t\tprint as decimal", false, true, true});
            _args.add({"-x", "\t\tprint as hexadecimal (default)", false, true, true});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            auto type = BleDataType::HEXADECIMAL;

            for (auto &o : _args.filterOptions(args)) {
                if (o == "-a") {
                    type = BleDataType::ASCII;
                } else if (o == "-b") {
                    type = BleDataType::BINARY;
                } else if (o == "-d") {
                    type = BleDataType::DECIMAL;
                } else if (o == "-x") {
                    type = BleDataType::HEXADECIMAL;
                } else {
                    std::cerr << "Unknown options: " << o << std::endl;
                    return EXIT_FAILURE;
                }
            }

            std::vector<std::string> values = _args.filterValues(args);

            if (values.size() != _args.getNumberOfRequiredArgs()) {
                std::cerr << "Invalid usage, see 'help read'" << std::endl;
                return EXIT_FAILURE;
            }

            BluetoothUUID service = values.at(0);
            BluetoothUUID characteristic = values.at(1);
            std::string vals = bt.read(service, characteristic);

            if (vals.empty()) {
                return EXIT_FAILURE;
            }

            Utils::printBleValue(vals, type);
            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
