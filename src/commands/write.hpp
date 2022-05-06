#ifndef WRITE_HPP
#define WRITE_HPP

#include "BleController.hpp"
#include "ICommand.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Write : public ICommand {
    public:
        Write()
        {
            _name = "write";
            _hint = "Write a characteristic";
            _usage = "service_uuid characteristic_uuid [-a] [-x] [-d] value";

            _args.add(
                {"service_uuid", "\tuuid of the service in which the characteristic is", true, false, false});
            _args.add({"characteristic_uuid", "uuid of the characteristic to read", true, false, false});
            _args.add({"-a", "\t\twrite as ascii", false, true, true});
            _args.add({"-x", "\t\twrite as hexadecimal (default)", false, true, true});
            _args.add({"-d", "\t\twrite as decimal", false, true, true});
            _args.add({"-b", "\t\twrite as binary", false, true, true});
            _args.add({"value", "\t\tvalue to write", true, false, false});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            auto type = BleDataType::HEXADECIMAL;

            for (auto &o : _args.filterOptions(args)) {
                if (o == "-a") {
                    type = BleDataType::ASCII;
                } else if (o == "-x") {
                    type = BleDataType::HEXADECIMAL;
                } else if (o == "-d") {
                    type = BleDataType::DECIMAL;
                } else if (o == "-b") {
                    type = BleDataType::BINARY;
                } else {
                    std::cerr << "Unknown options: " << o << std::endl;
                    return EXIT_FAILURE;
                }
            }

            std::vector<std::string> values = _args.filterValues(args);

            if (values.size() != _args.getNumberOfRequiredArgs()) {
                std::cerr << "Invalid usage, see 'help write'" << std::endl;
                return EXIT_FAILURE;
            }

            BluetoothUUID service = values.at(0);
            BluetoothUUID characteristic = values.at(1);
            ByteArray value = values.at(2);

            if (type == BleDataType::ASCII) {
                return bt.write(service, characteristic, value);
            } else if (type == BleDataType::HEXADECIMAL) {
                auto ret = Utils::parseHexStr(value);
                return bt.write(service, characteristic, ret.value());
            } else if (type == BleDataType::DECIMAL) {
                auto ret = Utils::parseDecStr(value);
                return bt.write(service, characteristic, ret.value());
            } else if (type == BleDataType::BINARY) {
                auto ret = Utils::parseBinStr(value);
                return bt.write(service, characteristic, ret.value());
            }

            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
