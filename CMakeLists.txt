cmake_minimum_required(VERSION 3.23)
project(montecarlo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

add_executable(mc_tests
    tests/test_european.cpp
    src/european.cpp)

target_include_directories(mc_tests PRIVATE include/mc third_party/random123/include)
target_link_libraries(mc_tests PRIVATE GTest::gtest_main)

include (GoogleTest)
gtest_discover_tests(mc_tests)

add_executable(mc_cli src/main.cpp src/european.cpp src/american.cpp)

target_include_directories(mc_cli PRIVATE include/mc third_party/random123/include)
target_link_libraries(mc_cli PRIVATE Threads::Threads)

