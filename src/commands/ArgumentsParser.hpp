#ifndef ARGUMENTSPARSER_HPP
#define ARGUMENTSPARSER_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace Command {
    struct Argument {
        std::string name;
        std::string hint;
        bool required;
        bool is_option;
        bool is_argname;
    };

    class ArgumentsParser {
    public:
        void add(Argument a)
        {
            if (a.name.empty()) {
                throw std::runtime_error("Argument name can't be empty");
            }

            _args.push_back(a);
        }

        bool empty()
        {
            return _args.empty();
        }

        std::vector<std::string> filterOptions(const std::vector<std::string> &all_args)
        {
            std::vector<std::string> filteredVector;

            std::copy_if(all_args.begin(), all_args.end(), std::back_inserter(filteredVector),
                [](std::string arg) { return arg.at(0) == '-'; });

            return filteredVector;
        }

        std::vector<std::string> filterValues(const std::vector<std::string> &all_args)
        {
            std::vector<std::string> filteredVector;

            std::copy_if(all_args.begin(), all_args.end(), std::back_inserter(filteredVector),
                [](std::string arg) { return arg.at(0) != '-'; });

            return filteredVector;
        }

        const std::vector<Argument> &getArgumentList() const
        {
            return _args;
        }

        std::size_t getNumberOfRequiredArgs() const
        {
            return std::count_if(_args.begin(), _args.end(), [](Argument a) { return a.required; });
        }

        std::size_t getNumberArgs() const
        {
            return _args.size();
        }

    private:
        std::vector<Argument> _args;
    };
} // namespace Command

#endif
