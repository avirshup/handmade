cmake_minimum_required(VERSION 3.31)
project(handmade)

set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_MODE=1")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE=1")

# dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# I guess if we're gonna test it, we have to compile it as a library
add_library(handmade_lib STATIC ${SOURCES})
target_compile_definitions(handmade_lib PRIVATE
        ASSET_PATH="${CMAKE_BINARY_DIR}/assets")
target_link_libraries(handmade_lib PUBLIC
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        spdlog::spdlog
        glm::glm)

# Create the main executable
add_executable(handmade src/main.cpp)
target_link_libraries(handmade PRIVATE handmade_lib)

# Assets in the build
# TODO: these don't seem to be updated automatically?
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})


# Test setup
find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(tests)

# # For syncing redraws to screen refresh:
#if (APPLE)
#    target_link_libraries(handmade
#            PRIVATE
#            "-framework CoreVideo"
#            "-framework CoreFoundation"
#    )
#endif ()