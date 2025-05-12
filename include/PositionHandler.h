#ifndef POSITION_HANDLER_H
#define POSITION_HANDLER_H

#include <map>
#include <string>

class PositionHandler {
public:
    static const std::map<std::string, std::string> nullPos;
    explicit PositionHandler(std::string fileName, std::istream& file);
    char advanceCharacter();
    bool advanceLine();
    char peek() const;
    void resetPos();
    [[nodiscard]] static std::string getWordFromLine(const std::map<std::string, std::string>& pos);
    [[nodiscard]] char getChar() const;
    [[nodiscard]] int getLineNumber() const;
    [[nodiscard]] std::map<std::string, std::string> getPos() const;

private:
    std::istream& file;
    int charPos;
    char currentChar;
    int line;
    std::string fileName;
    std::string lineText;
};



#endif // POSITION_HANDLER_H
