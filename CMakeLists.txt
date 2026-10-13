cmake_minimum_required(VERSION 3.23)
project(monte_carlo_simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mc src/main.cpp src/europe.cpp)

target_include_directories(mc PRIVATE 
    include
    random123/include
)

