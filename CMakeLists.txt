cmake_minimum_required(VERSION 3.28)
project(VIS)

set(CMAKE_CXX_STANDARD 17)

# Add the include directory to the include path
include_directories(include)

include(${PROJECT_SOURCE_DIR}/sources.cmake)


enable_testing()
add_subdirectory(tests)



add_executable(VIS src/main.cpp ${PROJECT_SOURCES})
