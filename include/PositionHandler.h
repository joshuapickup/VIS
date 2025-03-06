#ifndef POSITION_HANDLER_H
#define POSITION_HANDLER_H

#include <map>
#include <string>

class PositionHandler {
    public:
        static const std::map<std::string, std::string> nullPos;
        explicit PositionHandler(const std::string& name);
        char advanceCharacter();
        void advanceLine(const std::string& lineText);
        char peek() const;
        void resetPos();
        [[nodiscard]] char getChar() const;
        [[nodiscard]] std::map<std::string, std::string> getPos() const;

    private:
        int charPos;
        char currentChar;
        int line;
        std::string fileName;
        std::string lineText;

};



#endif // POSITION_HANDLER_H
