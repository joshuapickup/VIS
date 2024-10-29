#include <iostream>
#include <string>
#include <unordered_map>

class PositionHandler {
private:
    int charPos;              // Character position
    char currentChar;         // Current character
    int line;                 // Current line number
    std::string fileName;     // Name of the file
    std::string lineText;     // Text of the current line

public:
    // Constructor to initialize the PositionHandler
    PositionHandler(const std::string& name)
        : charPos(-1), currentChar('\0'), line(-1), fileName(name), lineText("") {}

    // Get the current character
    char getChar() const {
        return currentChar;
    }

    // Advance to the next character
    void advanceCharacter() {
        if (charPos < static_cast<int>(lineText.length()) - 1) {
            charPos++;
            currentChar = lineText[charPos];
        } else {
            currentChar = '\0'; // Set currentChar to '\0' when at the end of the line
        }
    }

    // Advance to the next line
    void advanceLine(const std::string& lineText) {
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
    void resetPos() {
        charPos = -1;
        line = -1;
        currentChar = '\0'; // Reset current character
        lineText.clear(); // Clear the line text
    }

    // Get current position details
    std::unordered_map<std::string, std::string> getPos() const {
        return {
                {"name", fileName},
                {"line", std::to_string(line)},
                {"charPos", std::to_string(charPos)},
                {"lineText", lineText},
                {"character", std::string(1, currentChar)}
        };
    }
};