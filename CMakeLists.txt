cmake_minimum_required(VERSION 3.15)

project(bluerepl CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Add Warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_executable(bluerepl
    src/main.cpp

    src/utils.cpp
    src/utils.hpp

    src/Repl/Repl.cpp
    src/Repl/Repl.hpp
    src/Repl/LineReader.cpp
    src/Repl/LineReader.hpp
    src/Repl/AliasManager.cpp
    src/Repl/AliasManager.hpp

    src/Controllers/SimpleBLE/SimpleBleController.cpp
    src/Controllers/SimpleBLE/SimpleBleController.hpp
    src/BleController.hpp

    src/presets/Preset.h
    src/presets/PresetManager.cpp
    src/presets/PresetManager.hpp
    src/presets/PresetParser.cpp
    src/presets/PresetParser.hpp
    src/presets/PresetCommand.cpp
    src/presets/PresetCommand.hpp

    src/commands/ArgumentsParser.hpp
    src/commands/ICommand.hpp
    src/commands/clear.hpp
    src/commands/connect.hpp
    src/commands/disconnect.hpp
    src/commands/info.hpp
    src/commands/read.hpp
    src/commands/scan.hpp
    src/commands/write.hpp
    src/commands/notify.hpp
    src/commands/indicate.hpp
    src/commands/unsubscribe.hpp
    )

target_include_directories(bluerepl PUBLIC
    src
    lib/tomlplusplus/include
    )

# linenoise
add_library(linenoise STATIC
        lib/linenoise-mob/linenoise.c
    )

target_include_directories(linenoise PUBLIC lib/linenoise-mob)

# simpleBLE
add_subdirectory(lib/SimpleBLE)
include_directories(${SIMPLEBLE_INCLUDES})

add_subdirectory(lib/tabulate)

target_link_libraries(bluerepl simpleble-static linenoise fmt tabulate)
