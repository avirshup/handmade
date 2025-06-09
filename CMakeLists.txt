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
add_executable(handmade ${SOURCES})
target_link_libraries(handmade PRIVATE
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        spdlog::spdlog
        glm::glm)

# Assets in the build
# TODO: these don't seem to be updated automatically?
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
target_compile_definitions(handmade PRIVATE
        ASSET_PATH="${CMAKE_BINARY_DIR}/assets")

# # For syncing redraws to screen refresh:
#if (APPLE)
#    target_link_libraries(handmade
#            PRIVATE
#            "-framework CoreVideo"
#            "-framework CoreFoundation"
#    )
#endif ()