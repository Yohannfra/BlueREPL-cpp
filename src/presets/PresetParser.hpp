#ifndef PRESETPARSER_HPP
#define PRESETPARSER_HPP

#include "Preset.h"

#include <toml++/toml.h>

namespace Preset {
    class Parser {
    public:
        int parse(const std::string &fp, Preset &dest);

    private:
        void parseServices(toml::table services, Preset &dest);
        void parseCharacteristic(const std::string &name,
            toml::node_view<toml::node> characteristic, Service &service);
    };
} // namespace Preset

#endif
