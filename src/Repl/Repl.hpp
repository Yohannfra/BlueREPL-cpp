#ifndef REPL_HPP
#define REPL_HPP

#include "BleController.hpp"
#include "Repl/AliasManager.hpp"
#include "commands/ICommand.hpp"

#include <cstddef>
#include <optional>
#include <string>

extern "C" {
#include "linenoise.h"
}

class Repl {
public:
    explicit Repl(BleController &bt);
    ~Repl();
    int run();

    static std::optional<std::string> get_line(int last_exit_code = EXIT_SUCCESS);

private:
    int printHelp(std::vector<std::string> &args);
    int runAliasCommand(const std::vector<std::string> &args);
    BleController &_bt;
    std::pair<std::string, std::string> _aliases;
    AliasManager _aliasManager;
};

#endif
