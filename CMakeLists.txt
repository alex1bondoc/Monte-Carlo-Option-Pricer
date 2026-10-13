cmake_minimum_required(VERSION 3.23)
project(montecarlo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(mc_cli src/main.cpp)
target_include_directories(mc_cli PRIVATE include third_party/random123/include)
target_link_libraries(mc_cli PRIVATE Threads::Threads)
