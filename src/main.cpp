#include "Controllers/SimpleBLE/SimpleBleController.hpp"
#include "Repl/Repl.hpp"
#include "presets/PresetManager.hpp"
#include "utils.hpp"

#include <iostream>
#include <functional>
#include <string>

static int printUsage(int ret_code)
{
    std::cout << "USAGE: ./bluerepl [preset.toml]" << std::endl;
    return ret_code;
}

int main(int argc, const char *argv[])
{
    Preset::Manager prm;
    Repl repl;

    if (argc > 2) {
        return printUsage(EXIT_FAILURE);
    } else if (argc == 2 && std::string(argv[1]) == "-h") {
        return printUsage(EXIT_SUCCESS);
    }

    if (argc == 2) {
        if (prm.loadFromFile(argv[1]) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
        if (repl.loadPreset(prm) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }

    int ret;
    try {
        BleController *bt = new SimpleBLEController();
        repl.setBleController(bt);
        ret = repl.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception catched" << std::endl;
        return EXIT_FAILURE;
    }
    return ret;
}
