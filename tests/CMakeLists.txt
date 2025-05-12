# Add GoogleTest (manually downloaded)
add_subdirectory(${PROJECT_SOURCE_DIR}/googletest ${CMAKE_BINARY_DIR}/googletest-build)


include(${PROJECT_SOURCE_DIR}/sources.cmake)

# Enable code coverage flags only for test builds
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Adding coverage flags for vis_tests")
    add_compile_options(-O0 --coverage)
    add_link_options(--coverage)
endif()

# Define the test executable
add_executable(vis_tests
        TestToken.cpp
        TestLexer.cpp
        TestParser.cpp
        TestInterpreter.cpp
        ${PROJECT_SOURCES}
        TestHelpers.h
)

# Link GoogleTest and its main entry point
target_link_libraries(vis_tests
        gtest
        gtest_main
        gmock
        gmock_main
)

# Enable automatic test discovery in CLion
include(GoogleTest)
gtest_discover_tests(vis_tests)
