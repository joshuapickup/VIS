#include <string>
#include <unordered_map>
#include "PositionHandler.h"


PositionHandler::PositionHandler(const std::string& fileName):  charPos(-1),
                                                            currentChar('\0'),
                                                            line(-1),
                                                            fileName(fileName),
                                                            lineText("") {}

char PositionHandler::getChar() const {
    return currentChar;
}

// advance to the next character
char PositionHandler::advanceCharacter() {
    if (charPos < static_cast<int>(lineText.length()) - 1) {
        charPos++;
        currentChar = lineText[charPos];
    }
    else {
        currentChar = '\0'; // set currentChar to '\0' when at the end of the line
    }
    return currentChar;
}

// advance to the next line
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

// returns next character without advancing
char PositionHandler::peek() const{
    if (charPos < static_cast<int>(lineText.length()) - 1) {
        return lineText[charPos+1];
    }
    else {
        return '\0';
    }
}

// reset position
void PositionHandler::resetPos() {
    charPos = -1;
    line = -1;
    currentChar = '\0';
    lineText.clear();
}

// get current position details
std::map<std::string, std::string> PositionHandler::getPos() const {
    return {
            {"name", fileName},
            {"line", std::to_string(line)},
            {"charPos", std::to_string(charPos)},
            {"lineText", lineText},
            {"character", std::string(1, currentChar)}
    };
}
