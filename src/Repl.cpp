#include "Repl.hpp"

#include "commands/help.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

static constexpr const char *HISTORY_FILE_PATH = ".blrepl_history.txt";

static const std::string ALL_COMMANDS[] = {
    "connect",
    "disconnect",
    "help",
    "info",
    "print_config",
    "read",
    "scan",
    "subscribe",
    "unsubscribe",
    "write",
};

static void completion(const char *buf, linenoiseCompletions *lc)
{
    for (auto &command : ALL_COMMANDS) {
        if (buf[0] == command[0]) {
            linenoiseAddCompletion(lc, command.c_str());
        }
    }
}

Repl::Repl(BleController &bt) : _bt(bt)
{
    linenoiseSetCompletionCallback(completion);
    linenoiseSetHintsCallback(NULL);
    linenoiseHistoryLoad(HISTORY_FILE_PATH);
}

Repl::~Repl()
{
    std::cout << "Bye" << std::endl;
}

void Repl::print_help() const
{
}

std::optional<std::string> Repl::get_line()
{
    std::string line;

    char *buff = linenoise(">> ");
    if (buff == NULL) {
        return {};
    }
    line = buff;
    free(buff);
    return line;
}

int Repl::run()
{
    std::cout << "Type help to list available commands" << std::endl;

    std::optional<std::string> readline;
    while ((readline = Repl::get_line()).has_value()) {
        std::string line = readline.value();
        if (!(line[0] != '\0')) {
            continue;
        }

        linenoiseHistoryAdd(line.c_str());
        linenoiseHistorySave(HISTORY_FILE_PATH);

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        std::vector<std::string> args;
        std::string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }

        if (cmd == "help") {
            Command::Help::run(args);
        } else if (cmd == "scan") {
            Command::Scan::run(_bt, args);
        } else if (cmd == "subscribe") {
        } else if (cmd == "connect") {
            Command::Connect::run(_bt, args);
        } else if (cmd == "disconnect") {
            Command::Disconnect::run(_bt, args);
        } else if (cmd == "info") {
            Command::Info::run(_bt, args);
        } else if (cmd == "read") {
        } else if (cmd == "write") {
        } else if (cmd == "subscribe") {
        } else if (cmd == "unsubscribe") {
        } else if (cmd == "print_config") {
        } else if (cmd == "subscribe") {
        } else {
            std::cerr << "Unknown command: " << cmd << std::endl;
        }
    }

    return 0;
}
