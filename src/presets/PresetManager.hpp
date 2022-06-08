#ifndef PRESETMANAGER_HPP
#define PRESETMANAGER_HPP

#include "Preset.h"
#include "PresetParser.hpp"

#include <filesystem>
#include <string>

namespace Preset {
    class Manager {
    public:
        Manager();

        int loadFromFile(const std::filesystem::path &fp);
        void print() const;
        Preset &getPreset();

    private:
        Preset _preset;
        Parser _parser;
    };
} // namespace Preset

#endif
