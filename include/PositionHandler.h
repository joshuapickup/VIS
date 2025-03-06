#ifndef POSITION_HANDLER_H
#define POSITION_HANDLER_H

#include <string>
#include <unordered_map>


class PositionHandler {
    public:
        explicit PositionHandler(const std::string& name);
        char advanceCharacter();
        void advanceLine(const std::string& lineText);
        char peek() const;
        void resetPos();
        [[nodiscard]] char getChar() const;
        [[nodiscard]] std::unordered_map<std::string, std::string> getPos() const;

    private:
        int charPos;
        char currentChar;
        int line;
        std::string fileName;
        std::string lineText;

};



#endif // POSITION_HANDLER_H
