#include <iostream>
#include <string>
#include <unordered_map>
#include "PositionHandler.h"


PositionHandler::PositionHandler(const std::string& name)
        : charPos(-1),currentChar('\0'), line(-1), fileName(name), lineText("") {}

// Get the current character
char PositionHandler::getChar() const {
    return currentChar;
}

// Advance to the next character
void PositionHandler::advanceCharacter() {
    if (charPos < static_cast<int>(lineText.length()) - 1) {
        charPos++;
        currentChar = lineText[charPos];
    } else {
        currentChar = '\0'; // Set currentChar to '\0' when at the end of the line
    }
}

// Advance to the next line
void PositionHandler::advanceLine(const std::string& lineText) {
    this->lineText = lineText;
    line++;
    charPos = 0;

    if (!lineText.empty()) {
        currentChar = lineText[charPos]; // Set the current character
    } else {
        currentChar = '\0'; // Set to null character if the line is empty
    }
}

// Reset position
void PositionHandler::resetPos() {
    charPos = -1;
    line = -1;
    currentChar = '\0'; // Reset current character
    lineText.clear(); // Clear the line text
}

// Get current position details
std::unordered_map<std::string, std::string> PositionHandler::getPos() const {
    return {
            {"name", fileName},
            {"line", std::to_string(line)},
            {"charPos", std::to_string(charPos)},
            {"lineText", lineText},
            {"character", std::string(1, currentChar)}
    };
}
