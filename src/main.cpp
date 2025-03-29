#include <iostream>
#include <fstream>
#include <string>

#include "PositionHandler.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

//TODO implement logic inside interpreter calls and call Inrepreter::interpretFile() or Inrepreter::interpretConsole()
int runFile(const std::string &filename) {
    auto interpreter = Interpreter(filename);
    std::cout << "full done" << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { // check filename argument is given
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    else {
        return runFile(argv[1]);
    }
}
