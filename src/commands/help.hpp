#ifndef HELP_HPP
#define HELP_HPP

#include "connect.hpp"
#include "disconnect.hpp"
#include "info.hpp"
#include "scan.hpp"

namespace Command {

    class Help {
    public:
        static void print_help()
        {
            std::cout << "help [command]" << '\n';
            std::cout << "\nDescription:\n\tprint help text about all commands "
                         "or the help text of a specific [command]"
                      << std::endl;
        }

        static int run(std::vector<std::string> &args)
        {
            if (args.empty()) {
                std::cout << "Commands:" << '\n';
                std::cout << "connect:" << '\n';
                std::cout << "disconnect:" << '\n';
                std::cout << "help:" << '\n';
                std::cout << "info:" << '\n';
                std::cout << "print_config:" << '\n';
                std::cout << "read:" << '\n';
                std::cout << "scan:" << '\n';
                std::cout << "subscribe:" << '\n';
                std::cout << "unsubscribe:" << '\n';
                std::cout << "write:" << '\n';
                std::cout << std::endl;
            } else {
                for (auto &arg : args) {
                    if (arg == "help") {
                        Help::print_help();
                    } else if (arg == "scan") {
                        Scan::print_help();
                    } else if (arg == "subscribe") {
                    } else if (arg == "disconnect") {
                        Disconnect::print_help();
                    } else if (arg == "connect") {
                        Connect::print_help();
                    } else if (arg == "info") {
                        Info::print_help();
                    } else if (arg == "read") {
                    } else if (arg == "write") {
                    } else if (arg == "subscribe") {
                    } else if (arg == "unsubscribe") {
                    } else if (arg == "print_config") {
                    } else if (arg == "subscribe") {
                    } else {
                        std::cerr << "Unknow command: " << arg << std::endl;
                    }
                }
            }
            return EXIT_SUCCESS;
        }
    };

} // namespace Command

#endif
