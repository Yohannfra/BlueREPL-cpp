#ifndef REPL_HPP
#define REPL_HPP

#include "BleController.hpp"
#include "LineReader.hpp"
#include "Repl/AliasManager.hpp"
#include "commands/ICommand.hpp"
#include "presets/PresetManager.hpp"

#include <cstddef>
#include <optional>
#include <string>

class Repl {
public:
    Repl();
    int setBleController(BleController *bt);
    int loadPreset(Preset::Manager &prm);
    ~Repl();
    int run();

#ifndef GOOGLE_TESTS_BUILD // to make these functions public when testing
private:
#endif

    std::vector<std::string> getAllCommandsNames();
    int printHelp(std::vector<std::string> &args);
    int runAliasCommand(const std::vector<std::string> &args);
    int runCommand(const std::string &line, bool &quit);

    BleController *_bt = nullptr;
    AliasManager _aliasManager;
    LineReader _reader;
    std::vector<Command::ICommand *> _commands;
    std::unique_ptr<Preset::Manager> _prm = nullptr;
    bool _started = false;
};

#endif
