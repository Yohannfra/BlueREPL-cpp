#include "BleController.hpp"
#include "Repl.hpp"
#include "linenoise.h"
#include "nlohmann/json.hpp"
#include "Controllers/SimpleBLE/SimpleBleController.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <toml++/toml.h>

using namespace nlohmann;

std::optional<json> load_json(const std::filesystem::path fp)
{
    if (fp.extension() != ".json") {
        std::cerr << "Error: '" << fp.string() << "' is not a json file" << std::endl;
        return {};
    }

    std::cout << fp.string() << std::endl;
    if (!std::filesystem::exists(fp)) {
        std::cerr << fp.string() << ": No such file" << std::endl;
        return {};
    }

    std::ifstream i(fp);
    json j;
    i >> j;

    return j;
}

int main(int argc, const char **argv)
{
    if (argc == 2) {
        auto res = load_json(argv[1]);
        if (!res.has_value()) {
            exit(EXIT_FAILURE);
        }
        std::cout << res.value() << std::endl;
    }

    try {
        BleController *bt = new SimpleBLEController();
        Repl repl(*bt);
        repl.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception catched. aborting" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
