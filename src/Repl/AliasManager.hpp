#ifndef ALIASMANAGER_HPP
#define ALIASMANAGER_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

class AliasManager {
public:
    int add(const std::string &key, const std::string &value);

    int add(std::vector<std::pair<std::string, std::string>> aliases);

    int printAliases() const;

    int printAlias(const std::string &key) const;

    int deleteAlias(std::string key);

    std::vector<std::string> replaceWithAliases(const std::vector<std::string> &in);

    const std::map<std::string, std::string> &getAliases() const;

private:
    void printAlias(const std::string &key, const std::string &value) const;
    std::map<std::string, std::string> _aliases;
};

#endif
