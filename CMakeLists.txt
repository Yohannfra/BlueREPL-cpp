cmake_minimum_required(VERSION 3.15)

project(blerepl CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(blerepl
    src/main.cpp

    src/utils.cpp
    src/utils.hpp

    src/Repl.cpp
    src/Repl.hpp
    src/Repl/AliasManager.cpp
    src/Repl/AliasManager.hpp

    src/Controllers/SimpleBLE/SimpleBleController.cpp
    src/Controllers/SimpleBLE/SimpleBleController.hpp
    src/BleController.hpp

    src/commands/connect.hpp
    src/commands/help.hpp
    src/commands/scan.hpp
    )

target_include_directories(blerepl PUBLIC
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

target_link_libraries(blerepl simpleble-static linenoise fmt)
