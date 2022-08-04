#ifndef LINEREADER_HPP
#define LINEREADER_HPP

#include <optional>
#include <string>

class LineReader {
public:
    LineReader();
    static std::optional<std::string> get(int last_exit_code = 0);

    void addInHistory(const std::string &s);

private:
    std::string getHistoryFilePath() const;
};
#endif
