cmake_minimum_required(VERSION 3.23)
project(monte_carlo_simulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED)

# biblioteca cu codul de calcul
add_library(mc_lib src/europe.cpp src/america.cpp)
target_include_directories(mc_lib PUBLIC
    include
    random123/include
)
target_link_libraries(mc_lib PUBLIC Threads::Threads Eigen3::Eigen)

# programul
add_executable(mc src/main.cpp)
target_link_libraries(mc PRIVATE mc_lib)

# testele
enable_testing()
add_executable(mc_tests tests/test1.cpp)
target_link_libraries(mc_tests PRIVATE mc_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mc_tests)