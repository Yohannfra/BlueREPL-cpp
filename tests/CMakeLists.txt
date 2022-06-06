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
)

target_include_directories(
  bluerepl_test PUBLIC
  ../src
    )

# simpleBLE
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib/SimpleBLE ${CMAKE_BINARY_DIR}/simpleble)
include_directories(${SIMPLEBLE_INCLUDES})

target_link_libraries(
  bluerepl_test
  simpleble-static
  fmt
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(bluerepl_test)
