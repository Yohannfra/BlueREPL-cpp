#include "BleController.hpp"
#include "Controllers/SimpleBLE/SimpleBleController.hpp"
#include "Repl.hpp"
#include "linenoise.h"

#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <toml++/toml.h>

int main()
{
    int ret;

    try {
        BleController *bt = new SimpleBLEController();
        Repl repl(*bt);
        ret = repl.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception catched. aborting" << std::endl;
        return EXIT_FAILURE;
    }
    return ret;
}
