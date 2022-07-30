#include "Repl.hpp"

#include "commands/clear.hpp"
#include "commands/connect.hpp"
#include "commands/disconnect.hpp"
#include "commands/indicate.hpp"
#include "commands/info.hpp"
#include "commands/notify.hpp"
#include "commands/read.hpp"
#include "commands/scan.hpp"
#include "commands/unsubscribe.hpp"
#include "commands/write.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

int Repl::printHelp(std::vector<std::string> &args)
{
    if (args.size() == 1) {
        auto cmd = std::find_if(_commands.begin(), _commands.end(),
            [&args](Command::ICommand *c) { return c->getName() == args.at(0); });

        if (cmd == _commands.end()) {
            std::cerr << "Unknown command: " << args.at(0) << std::endl;
            return EXIT_FAILURE;
        }
        (*cmd)->printHelp();
        return EXIT_SUCCESS;
    }

    for (const auto &c : _commands) {
        const int nbTabsNeeded = 3 - static_cast<int>(c->getName().size() / 4);

        std::cout << c->getName();
        for (int i = 0; i < nbTabsNeeded; i++) {
            std::cout << '\t';
        }
        std::cout << c->getHint() << std::endl;
    }
    return EXIT_SUCCESS;
}

Repl::Repl() : _bt(nullptr)
{
    // register commands
    _commands.push_back(new Command::Connect);
    _commands.push_back(new Command::Clear);
    _commands.push_back(new Command::Disconnect);
    _commands.push_back(new Command::Info);
    _commands.push_back(new Command::Read);
    _commands.push_back(new Command::Scan);
    _commands.push_back(new Command::Notify);
    _commands.push_back(new Command::Indicate);
    _commands.push_back(new Command::Unsubscribe);
    _commands.push_back(new Command::Write);
}

int Repl::setBleController(BleController *bt)
{
    if (bt == nullptr) {
        std::cerr << "Ble controller set is null" << std::endl;
        return EXIT_FAILURE;
    }

    _bt = bt;
    return EXIT_SUCCESS;
}

int Repl::loadPreset(Preset::Manager &prm)
{
    _prm = std::make_unique<Preset::Manager>(prm);

    for (const auto &[k, v] : _prm->getPreset().aliases) {
        _aliasManager.add(k, v);
    }

    for (const auto &c : _prm->getPreset().commands) {
        auto all_cmds = this->getAllCommandsNames();
        if (std::find(all_cmds.begin(), all_cmds.end(), c.getName()) != all_cmds.end()) {
            std::cerr << "Duplicated command or alias: '" << c.getName() << "'"
                      << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

Repl::~Repl()
{
    if (_started) {
        std::cout << "Bye" << std::endl;
    }
}

int Repl::runAliasCommand(const std::vector<std::string> &args)
{
    if (args.empty()) {
        return _aliasManager.printAliases();
    } else if (args.size() == 1) {
        return _aliasManager.deleteAlias(args.at(0));
    } else if (args.size() == 2) {
        return _aliasManager.add(args.at(0), args.at(1));
    } else {
        std::cerr << "Unknown alias command" << std::endl;
        return EXIT_FAILURE;
    }
}

int Repl::runCommand(const std::string &line, bool &quit)
{
    std::vector<std::string> splitted_line_raw = Utils::splitInArgs(std::string(line));

    std::vector<std::string> splitted_line =
        _aliasManager.replaceWithAliases(splitted_line_raw);

    std::string cmd = splitted_line.at(0);

    auto args = splitted_line;
    args.erase(args.begin());

    // Repl built in
    if (cmd == "quit" || cmd == "exit") {
        quit = true;
        return EXIT_SUCCESS;
    } else if (cmd == "help") {
        return this->printHelp(args);
    } else if (cmd == "alias") {
        auto raw_args = splitted_line_raw;
        raw_args.erase(raw_args.begin());
        return this->runAliasCommand(raw_args);
    } else if (cmd == "preset") {
        if (_prm) {
            _prm->print();
            return EXIT_SUCCESS;
        } else {
            std::cerr << "No preset loaded" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // commands
    for (auto &c : _commands) {
        if (cmd == c->getName()) {
            return c->run(args, *_bt);
        }
    }

    // preset
    if (_prm) {
        for (auto &c : _prm->getPreset().commands) {
            if (cmd == c.getName()) {
                return c.runAction(*_bt, _commands);
            }
        }
    }

    std::cerr << "Unknown command: " << cmd << std::endl;
    return EXIT_FAILURE;
}

int Repl::run()
{
    std::cout << "Type help to list available commands" << std::endl;
    int last_exit_code = EXIT_SUCCESS;
    bool quit = false;

    _started = true;

    std::optional<std::string> readline;
    while ((readline = LineReader::get(last_exit_code)).has_value()) {
        std::string line = readline.value();
        if (!(line[0] != '\0')) {
            continue;
        }
        last_exit_code = runCommand(line, quit);
        if (quit) {
            break;
        }
    }
    return last_exit_code;
}

std::vector<std::string> Repl::getAllCommandsNames()
{
    std::vector<std::string> all_cmds;

    for (const auto &c : _commands) {
        all_cmds.push_back(c->getName());
    }

    for (const auto &[k, _] : _aliasManager.getAliases()) {
        all_cmds.push_back(k);
    }

    return all_cmds;
}
