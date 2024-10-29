#include <iostream>
#include <fstream>
#include <string>

#include "PositionHandler.cpp"

int main(int argc, char* argv[]) {
    // Check if a filename argument was provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1]; // Name of the file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }
    PositionHandler positionHandler(filename); // Create a PositionHandler for the file

    if (!inputFile.is_open()) { // Check if the file opened successfully
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::string line;
    // Read the file line by line
    while (std::getline(inputFile, line)) {
        positionHandler.advanceLine(line); // Advance to the new line

        // Process each character in the line
        while (positionHandler.getChar() != '\0') { // Check if the current character is not null
            std::cout << "Line " << positionHandler.getPos().at("line")
                      << ", Char " << positionHandler.getPos().at("charPos")
                      << ": " << positionHandler.getChar() << std::endl;

            positionHandler.advanceCharacter(); // Advance to the next character
        }
    }

    // Close the file
    inputFile.close();

    return 0;
}