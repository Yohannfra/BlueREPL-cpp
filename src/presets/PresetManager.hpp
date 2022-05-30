#ifndef PRESETMANAGER_HPP
#define PRESETMANAGER_HPP

#include "Preset.h"
#include "PresetParser.hpp"

#include <string>

namespace Preset {
    class Manager {
    public:
        Manager();

        int load(const std::string &fp);
        void print() const;
        Preset &getPreset();

    private:
        Preset _preset;
        Parser _parser;
    };
} // namespace Preset

#endif
