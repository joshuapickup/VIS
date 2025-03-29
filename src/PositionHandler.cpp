#include <string>
#include "PositionHandler.h"
#include <fstream>
#include <utility>

const std::map<std::string, std::string> PositionHandler::nullPos = {
    {"name", "null"},
    {"line", "null"},
    {"charPos", "null"},
    {"lineText", "null"},
    {"character", "null"}
};

PositionHandler::PositionHandler(std::string fileName, std::ifstream &file):
file(file),
charPos(-1),
currentChar('\0'),
line(-1),
fileName(std::move(fileName)) {}

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

// advance to the next line returns false if line dosent exist
bool PositionHandler::advanceLine() {
    if (!file.is_open()) {
        return false;
    }
    if (std::getline(file, lineText)) {
        line++;
        charPos = 0;
        if (!lineText.empty()) {
            currentChar = lineText[charPos]; // Set the current character
        } else {
            currentChar = '\0'; // Set to null character if the line is empty
        }
        return true;
    }
    file.close();
    return false;
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

std::string PositionHandler::getWordFromLine(const std::map<std::string, std::string>& pos) {
    const auto lineTextIt = pos.find("lineText");
    const auto charPosIt = pos.find("charPos");
    if (lineTextIt == pos.end() || charPosIt == pos.end()) {return "";}
    const std::string& lineText = lineTextIt->second;
    const int position = std::stoi(charPosIt->second);
    if (position < 0 || position >= static_cast<int>(lineText.length())) {return "";}

    auto isWordChar = [](char c) {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.';
    };


    int start = position;
    while (start > 0 && isWordChar(lineText[start - 1])) {
        --start;
    }

    int end = position;
    while (end < static_cast<int>(lineText.length()) - 1 && isWordChar(lineText[end + 1])) {
        ++end;
    }

    return lineText.substr(start, end - start + 1);
}

char PositionHandler::getChar() const {
    return currentChar;
}

int PositionHandler::getLineNumber() const {
    return line;
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
