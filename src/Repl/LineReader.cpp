#include "LineReader.hpp"

#include "utils.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

extern "C" {
#include "linenoise.h"
}

std::string LineReader::getHistoryFilePath() const
{
    std::string homedir;


    char *tmp = getenv("HOME");
    if (tmp == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    } else {
        homedir = tmp;
    }

    return homedir +  "/.blrepl_history.txt";
}

LineReader::LineReader()
{
    linenoiseHistoryLoad(this->getHistoryFilePath().c_str());
    linenoiseSetMultiLine(1);
}

std::optional<std::string> LineReader::get(int last_exit_code)
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

void LineReader::addInHistory(const std::string &s)
{
    linenoiseHistoryAdd(s.c_str());
    linenoiseHistorySave(this->getHistoryFilePath().c_str());
}
