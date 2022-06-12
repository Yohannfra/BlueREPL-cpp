#ifndef CLEAR_HPP
#define CLEAR_HPP

#include "BleController.hpp"
#include "ICommand.hpp"
#include "linenoise.h"

#include <iostream>
#include <string>
#include <vector>

namespace Command {
    class Clear : public ICommand {
    public:
        Clear()
        {
            _name = "clear";
            _hint = "clear the screen";
        }

        int run([[maybe_unused]] std::vector<std::string> &args,
            [[maybe_unused]] BleController &bt) override
        {
            linenoiseClearScreen();
            return EXIT_SUCCESS;
        }
    };
} // namespace Command

#endif
