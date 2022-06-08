#ifndef PRESETPARSER_HPP
#define PRESETPARSER_HPP

#include "Preset.h"

#include <filesystem>
#include <toml++/toml.h>

namespace Preset {
    class Parser {
    public:
        int parseFile(const std::filesystem::path &fp, Preset &dest);

    private:
        int parseAll(toml::table &tbl, Preset &dest);
        void parseServices(toml::table services, Preset &dest);
        void parseCharacteristic(const std::string &name,
            toml::node_view<toml::node> characteristic, Service &service);
    };
} // namespace Preset

#endif
