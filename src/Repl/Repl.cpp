#include "Repl.hpp"

#include "commands/clear.hpp"
#include "commands/connect.hpp"
#include "commands/disconnect.hpp"
#include "commands/info.hpp"
#include "commands/read.hpp"
#include "commands/scan.hpp"
#include "commands/write.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

static constexpr const char *HISTORY_FILE_PATH = ".blrepl_history.txt";

static std::vector<Command::ICommand *> _commands;

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
        std::cout << c->getName() << "\t" << c->getHint() << std::endl;
    }
    return EXIT_SUCCESS;
}

void completion(const char *buf, linenoiseCompletions *lc)
{
    std::stringstream l(buf);

    std::vector<std::string> splitted;
    std::string tmp;
    while (l >> tmp) {
        splitted.push_back(tmp);
    }

    if (splitted.empty()) {
        return;
    }

    if (splitted.size() == 1) {
        for (const auto &c : _commands) {
            if (buf[0] == c->getName().at(0)) {
                linenoiseAddCompletion(lc, c->getName().c_str());
            }
        }
    }

// TODO more advanced completion
#if 0
    else {
        /* std::string val = *splitted.end().base(); */
        auto cmd = splitted.at(0);

        for (auto &c : _commands) {
            if (c->getName() == cmd) {
                for (auto &a : c->getArgumentList()) {
                    if ('-' == a.name.at(0)) {
                        linenoiseAddCompletion(lc, a.name.c_str());
                    }
                }
            }
        }
    }
#endif
}

Repl::Repl(BleController &bt) : _bt(bt)
{
    // init linenoise
    linenoiseSetCompletionCallback(completion);
    linenoiseSetHintsCallback(NULL);
    linenoiseHistoryLoad(HISTORY_FILE_PATH);
    linenoiseSetMultiLine(1);

    // register commands
    _commands.push_back(new Command::Connect);
    _commands.push_back(new Command::Clear);
    _commands.push_back(new Command::Disconnect);
    _commands.push_back(new Command::Info);
    _commands.push_back(new Command::Read);
    _commands.push_back(new Command::Scan);
    _commands.push_back(new Command::Write);
}

Repl::~Repl()
{
    std::cout << "Bye" << std::endl;
}

std::optional<std::string> Repl::get_line(int last_exit_code)
{
    std::string line;

    char *buff = linenoise(last_exit_code == EXIT_SUCCESS ? GREEN(">> ") : RED(">> "));

    if (buff == NULL) {
        return {};
    }
    line = buff;
    free(buff);
    return line;
}

int Repl::runAliasCommand(const std::vector<std::string> &args)
{
    if (args.empty()) {
        return _aliasManager.printAliases();
    } else if (args.size() == 1) {
        return _aliasManager.printAlias(args.at(0));
    } else if (args.size() == 2) {
        if (args.at(1).empty()) {
            return _aliasManager.deleteAlias(args.at(0));
        } else {
            return _aliasManager.add(args.at(0), args.at(1));
        }
    } else {
        std::cerr << "Unknown alias command" << std::endl;
        return EXIT_FAILURE;
    }
}

int Repl::run()
{
    std::cout << "Type help to list available commands" << std::endl;
    int last_exit_code = EXIT_SUCCESS;

    std::optional<std::string> readline;
    while ((readline = Repl::get_line(last_exit_code)).has_value()) {
        std::string line = readline.value();
        if (!(line[0] != '\0')) {
            continue;
        }

        linenoiseHistoryAdd(line.c_str());
        linenoiseHistorySave(HISTORY_FILE_PATH);

        std::vector<std::string> splitted_line =
            _aliasManager.replaceWithAliases(Utils::splitInArgs(std::string(line)));
        std::string cmd = splitted_line.at(0);

        auto args = splitted_line;
        args.erase(args.begin());

        bool found = false;

        // Built in
        if (cmd == "quit" || cmd == "exit") {
            break;
        } else if (cmd == "help") {
            last_exit_code = this->printHelp(args);
            found = true;
        } else if (cmd == "alias") {
            last_exit_code = this->runAliasCommand(args);
            found = true;
        }

        if (found) {
            continue;
        }

        for (auto &c : _commands) {
            if (cmd == c->getName()) {
                last_exit_code = c->run(args, _bt);
                found = true;
                break;
            }
        }

        if (!found) {
            std::cerr << "Unknown command: " << cmd << std::endl;
            last_exit_code = EXIT_FAILURE;
        }
    }
    return last_exit_code;
}
