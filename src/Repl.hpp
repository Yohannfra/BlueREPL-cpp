#ifndef REPL_HPP
#define REPL_HPP

#include "BleController.hpp"

#include <cstddef>
#include <optional>
#include <string>

extern "C" {
#include "linenoise.h"
}

class Repl {
public:
    Repl(BleController &bt);
    ~Repl();
    int run();

    static std::optional<std::string> get_line();

private:
    void print_help() const;

    BleController &_bt;
};

#endif
