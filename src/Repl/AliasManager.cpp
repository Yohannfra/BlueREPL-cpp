#include "AliasManager.hpp"

#include "utils.hpp"

#include <iostream>

void AliasManager::printAlias(const std::string &key, const std::string &value) const
{
    std::cout << key << "='" << value << "'" << std::endl;
}

int AliasManager::printAlias(const std::string &key) const
{
    if (_aliases.find(key) == _aliases.end()) {
        std::cerr << "'" << key << "' Not an alias" << std::endl;
        return EXIT_FAILURE;
    }

    /* this->printAlias(key, _aliases.at(key)); */
    return EXIT_SUCCESS;
}

int AliasManager::add(const std::string &key, const std::string &value)
{
    if (!Utils::isAsciiWord(key)) {
        std::cerr << "Key '" << key << "' is not an ascii word" << std::endl;
        return EXIT_FAILURE;
    }

    if (key == value) {
        std::cerr << "Key and value are the identical" << std::endl;
        return EXIT_FAILURE;
    }

    if (std::any_of(
            value.begin(), value.end(), [](char c) { return c == ' ' || c == '\t'; })) {
        std::cerr << "Values can't have blank spaces" << std::endl;
        return EXIT_FAILURE;
    }

    _aliases[key] = value;

    return this->printAlias(key);
}

int AliasManager::add(std::vector<std::pair<std::string, std::string>> new_aliases)
{
    for (const auto &[k, v] : new_aliases) {
        if (!this->add(k, v)) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int AliasManager::printAliases() const
{
    for (const auto &[k, v] : _aliases) {
        this->printAlias(k, v);
    }
    return EXIT_SUCCESS;
}

int AliasManager::deleteAlias(std::string key)
{
    if (_aliases.find(key) == _aliases.end()) {
        std::cerr << "'" << key << "' Not an alias" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Deleting alias " << key << std::endl;
    _aliases.erase(key);

    return EXIT_SUCCESS;
}

std::vector<std::string> AliasManager::replaceWithAliases(
    const std::vector<std::string> &base)
{
    bool done;
    std::vector<std::string> out;
    std::vector<std::string> in = base;

    do {
        done = true;
        out.clear();

        for (const auto &w : in) {
            auto res = _aliases.find(w);

            if (res == _aliases.end()) {
                out.push_back(w);
            } else {
                out.push_back((*res).second);
                done = false;
            }
        }
        in = out;
    } while (!done);

    return out;
}

const std::map<std::string, std::string> &AliasManager::getAliases() const
{
    return _aliases;
}
