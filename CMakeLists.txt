cmake_minimum_required(VERSION 3.28)
project(VIS)

set(CMAKE_CXX_STANDARD 17)

# Add the include directory to the include path
include_directories(include)

set(SOURCES src/main.cpp
            src/PositionHandler.cpp
            src/Lexer.cpp
            src/Error.cpp
            src/Interpreter.cpp
            src/Parser.cpp
            src/Node.cpp
            src/Token.cpp
            src/Literal.cpp
            src/Context.cpp
)

add_executable(VIS ${SOURCES})