cmake_minimum_required(VERSION 3.14)
project(my_project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  bluerepl_test

  # utils
  src/test_utils.cpp
  ../src/utils.cpp
  ../src/utils.hpp

  # presets
  src/test_presets.cpp
  ../src/presets/Preset.h
  ../src/presets/PresetCommand.cpp
  ../src/presets/PresetCommand.hpp
  ../src/presets/PresetManager.cpp
  ../src/presets/PresetManager.hpp
  ../src/presets/PresetParser.cpp
  ../src/presets/PresetParser.hpp

  # repl
  src/test_repl.cpp
  ../src/Repl/AliasManager.cpp
  ../src/Repl/AliasManager.hpp
  ../src/Repl/LineReader.cpp
  ../src/Repl/LineReader.hpp
  ../src/Repl/Repl.cpp
  ../src/Repl/Repl.hpp
)

target_include_directories(
  bluerepl_test PUBLIC
  ../src
  ../lib/tomlplusplus/include
    )

target_compile_definitions(bluerepl_test PUBLIC
    GOOGLE_TESTS_BUILD=1
    )

# simpleBLE
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib/SimpleBLE ${CMAKE_BINARY_DIR}/simpleble)
include_directories(${SIMPLEBLE_INCLUDES})

# linenoise
add_library(linenoise STATIC
        ${CMAKE_CURRENT_LIST_DIR}/../lib/linenoise-mob/linenoise.c
    )
target_include_directories(linenoise PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../lib/linenoise-mob)

# tabulate
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib/tabulate ${CMAKE_BINARY_DIR}/tabulate)

target_link_libraries(
  bluerepl_test
  simpleble-static
  tabulate
  fmt
  gtest_main
  linenoise
)

include(GoogleTest)
gtest_discover_tests(bluerepl_test)
