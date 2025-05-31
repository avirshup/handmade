cmake_minimum_required(VERSION 3.31)
project(handmade)

set(CMAKE_CXX_STANDARD 23)

# dependencies
find_package(SDL2 REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")


include_directories(handmade ${SDL2_INCLUDE_DIRS})
add_executable(handmade ${SOURCES})
target_link_libraries(handmade PRIVATE ${SDL2_LIBRARIES} spdlog::spdlog)

# # For syncing redraws to screen refresh:
#if (APPLE)
#    target_link_libraries(handmade
#            PRIVATE
#            "-framework CoreVideo"
#            "-framework CoreFoundation"
#    )
#endif ()