#ifndef PRESET_HPP
#define PRESET_HPP

#include "PresetCommand.hpp"

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Preset {
    struct Characteristic {
        std::string name;
        std::string uuid;
    };

    struct Service {
        std::string name;
        std::string uuid;
        std::vector<Characteristic> characteristics;
    };

    struct Preset {
        std::string fp;
        std::string name;
        bool autoconnect;
        std::string mac;
        std::vector<Service> services;
        std::map<std::string, std::string> aliases;
        std::vector<Command> commands;
    };
} // namespace Preset

#endif
