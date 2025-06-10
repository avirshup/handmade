file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(handmade_tests ${TEST_SOURCES})
target_link_libraries(handmade_tests PRIVATE
        GTest::GTest
        GTest::Main
        handmade_lib
)


# test registration and setup
# Q: what is the relationship from this to the "handmade_tests"
#    executable defined above?
include(GoogleTest)
gtest_discover_tests(handmade_tests) 