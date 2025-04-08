#include <iostream>
#include <fstream>
#include <string>

#include "Interpreter.h"

int main(int argc, char* argv[]) {
    if (argc < 2) { // check filename argument is given
        std::cerr << "Usage: " << argv[0] << " <filename> [--verbose]" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    bool verbose = false;
    if (argc >= 3) {
        std::string flag = argv[2];
        if (flag == "--verbose" || flag == "-v") {
            verbose = true;
        }
        else {
            std::cerr << "Unknown option: " << flag << std::endl;
            std::cerr << "Usage: " << argv[0] << " <filename> [--verbose]" << std::endl;
            return 1;
        }
    }
    auto interpreter = Interpreter(filename, verbose);
    return 0;
}
