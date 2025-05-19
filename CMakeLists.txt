cmake_minimum_required(VERSION 3.31)
project(handmade)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(handmade ${SDL2_INCLUDE_DIRS})

add_executable(handmade main.cpp)
target_link_libraries(handmade ${SDL2_LIBRARIES})