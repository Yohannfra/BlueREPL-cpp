#include "LineReader.hpp"

#include "utils.hpp"

extern "C" {
#include "linenoise.h"
}

static constexpr const char *HISTORY_FILE_PATH = ".blrepl_history.txt";

LineReader::LineReader()
{
    linenoiseHistoryLoad(HISTORY_FILE_PATH);
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
    linenoiseHistorySave(HISTORY_FILE_PATH);
}
