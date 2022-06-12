#ifndef INDICATE_HPP
#define INDICATE_HPP

#include "BleController.hpp"
#include "ICommand.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Indicate : public ICommand {
    public:
        Indicate()
        {
            _name = "indicate";
            _hint = "Subscribe to a characteristic indications and print it's value when "
                    "it gets updated";
            _usage = "service_uuid characteristic_uuid [-a] [-x] [-d]";

            _args.add(
                {"service_uuid", "\tuuid of the service in which the characteristic is",
                    true, false, false});
            _args.add({"characteristic_uuid",
                "uuid of the characteristic to subscribe to", true, false, false});
            _args.add({"-a", "\t\tprint as ascii", false, true, true});
            _args.add({"-b", "\t\tprint as binary", false, true, true});
            _args.add({"-d", "\t\tprint as decimal", false, true, true});
            _args.add({"-x", "\t\tprint as hexadecimal (default)", false, true, true});
        }

        int run(std::vector<std::string> &args, BleController &bt) override
        {
            std::function<void(ByteArray)> cb = Utils::printAsHex;

            for (auto &o : _args.filterOptions(args)) {
                if (o == "-a") {
                    cb = Utils::printAsAscii;
                } else if (o == "-b") {
                    cb = Utils::printAsBinary;
                } else if (o == "-d") {
                    cb = Utils::printAsDec;
                } else if (o == "-x") {
                    cb = Utils::printAsHex;
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
            return bt.indicate(service, characteristic, //
                [service, characteristic, cb](ByteArray bytes) {
                    std::cout << "Indication from " << service << " " << characteristic
                              << " :";
                    cb(bytes);
                } //
            );
        }
    };
} // namespace Command

#endif
