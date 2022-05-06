#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "ArgumentsParser.hpp"
#include "BleController.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class ICommand {
    public:
        void printHelp()
        {
            std::cout << BOLD("Usage: ") << _name << " " << _usage << '\n';

            std::cout << BOLD("\nDescription:\n\t") << _hint << '\n';

            if (!_args.empty()) {
                std::cout << BOLD("\nArguments:") << '\n';
                for (auto &arg : _args.getArgumentList()) {
                    std::cout << "\t" << arg.name << "\t" << arg.hint << '\n';
                }
            }

            if (!_help.empty()) {
                std::cout << BOLD("\nHelp: ") << "\n\t";
                std::cout << _help << '\n';
            }

            std::cout << std::endl;
        }

        virtual int run(std::vector<std::string> &arg, BleController &bt) = 0;

        const std::string &getName() const
        {
            return _name;
        }

        const std::string &getHint() const
        {
            return _hint;
        }

        const std::vector<Argument> &getArgumentList() const
        {
            return _args.getArgumentList();
        }

    protected:
        std::string _name;
        std::string _usage;
        std::string _help;
        std::string _hint;
        ArgumentsParser _args;
    };
} // namespace Command

#endif
