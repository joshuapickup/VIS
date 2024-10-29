#ifndef POSITION_HANDLER_H
#define POSITION_HANDLER_H

#include <string>
#include <unordered_map>


class PositionHandler {
    public:
        explicit PositionHandler(const std::string& name);

        char getChar() const;

        void advanceCharacter();

        void advanceLine(const std::string& lineText);

        void resetPos();

        std::unordered_map<std::string, std::string> getPos() const;

    private:
        int charPos;              // Character position
        char currentChar;         // Current character
        int line;                 // Current line number
        std::string fileName;     // Name of the file
        std::string lineText;     // Text of the current line

};



#endif // POSITION_HANDLER_H
