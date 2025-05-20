cmake_minimum_required(VERSION 3.31)
project(handmade)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(handmade ${SDL2_INCLUDE_DIRS})
include_directories(src)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(handmade ${SOURCES})
target_link_libraries(handmade ${SDL2_LIBRARIES})