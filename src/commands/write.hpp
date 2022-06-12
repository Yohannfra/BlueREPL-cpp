#ifndef WRITE_HPP
#define WRITE_HPP

#include "BleController.hpp"
#include "ICommand.hpp"
#include "utils.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Command {
    class Write : public ICommand {
    private:
    public:
        Write()
        {
            _name = "write";
            _hint = "Write a characteristic";
            _usage = "service_uuid characteristic_uuid value";

            _args.add(
                {"service_uuid", "\tuuid of the service in which the characteristic is",
                    true, false, false});
            _args.add({"characteristic_uuid", "uuid of the characteristic to read", true,
                false, false});
            _args.add({"value", "\t\tvalue to write", true, false, false});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            std::vector<std::string> values = _args.filterValues(args);

            if (values.size() != _args.getNumberOfRequiredArgs()) {
                std::cerr << "Invalid usage, see 'help write'" << std::endl;
                return EXIT_FAILURE;
            }

            BluetoothUUID service = values.at(0);
            BluetoothUUID characteristic = values.at(1);
            auto payloads = Utils::tokenizeLine(values.at(2));

            if (payloads.empty()) {
                std::cerr << "Empty or invalid payload(s)" << std::endl;
                return EXIT_FAILURE;
            }

            for (auto &[v, type] : payloads) {
                if (type == BleDataType::ASCII) {
                    return bt.write(service, characteristic, v);
                } else if (type == BleDataType::HEXADECIMAL) {
                    auto ret = Utils::parseHexStr(v);
                    return bt.write(service, characteristic, ret.value());
                } else if (type == BleDataType::DECIMAL) {
                    auto ret = Utils::parseDecStr(v);
                    return bt.write(service, characteristic, ret.value());
                } else if (type == BleDataType::BINARY) {
                    auto ret = Utils::parseBinStr(v);
                    return bt.write(service, characteristic, ret.value());
                }
            }

            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
