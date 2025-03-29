#include "Lexer.h"

#include <iostream>

#include "Error.h"


const std::unordered_set<std::string> Lexer::KEYWORDS = {
    "var","and","or", "not", "if", "else", "while", "for", "func"
};
const std::string Lexer::DIGITS = "0123456789";
const std::string Lexer::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
const std::string Lexer::LETTERS_DIGITS = Lexer::LETTERS + Lexer::DIGITS;
const std::string Lexer::OPERATORS = "+-*/";
const char Lexer::COMMENT = '~';

Lexer::Lexer(PositionHandler& positionHandler) : positionHandler(positionHandler) {}

Token Lexer::makeOperatorToken(const char character) const {
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    switch(character) {
        case '+':
            if (positionHandler.peek() == '+') {
                positionHandler.advanceCharacter();
                return Token(TokenType::INCREMENT, pos);
            }
            return Token(TokenType::PLUS, pos);
        case '-':
            if (positionHandler.peek() == '-') {
                positionHandler.advanceCharacter();
                return Token(TokenType::DECREMENT, pos);
            }
            return Token(TokenType::MINUS, pos);
        case '*': return Token(TokenType::MUL, pos);
        case '/': return Token(TokenType::DIV, pos);
        default: throw LexerError("operator token cant be created no case defined for character >>>" +
                                            std::string(1, character) + "<<<");
    }
}

Token Lexer::makeNumberToken (char character) const{// loop through line until next char isnt digit
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    bool dotFlag = false;
    std::string stringNum;
    stringNum += character;
    char peekChar = this->positionHandler.peek();
    while (peekChar != '\0' and (Lexer::DIGITS.find(peekChar) != std::string::npos or peekChar == '.')){
        character = this->positionHandler.advanceCharacter();
        if (character == '.') {
            std::map<std::string, std::string> position = positionHandler.getPos();
            if (!dotFlag) {dotFlag = true;}
            else {
                const std::string word = PositionHandler::getWordFromLine(position);
                throw IllegalCharError("\nIllegal Number >>> " + word + " <<<\n"+
                    "on line: " + std::to_string(stoi(position["line"]) + 1) +
                    " of file: " + position["name"] + "\n" +
                    "{" + position["lineText"] + "}");
            }
        }
        stringNum += character;
        peekChar = this->positionHandler.peek();
    }
    if (dotFlag) {
        return Token(TokenType::FLOAT, pos, std::stof(stringNum));
    }
    else {
        return Token(TokenType::INT, pos, std::stoi(stringNum));
    }
}

Token Lexer::makeIdentifierToken(char character) const{
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    std::string identifierString;
    identifierString += character;
    do {
        char peekChar = this->positionHandler.peek();
        if (peekChar != '\0' and Lexer::LETTERS_DIGITS.find(peekChar) != std::string::npos) {
            character = this->positionHandler.advanceCharacter();
            identifierString += character;
        }
        else {break;}
    } while (true);
    if (Lexer::KEYWORDS.find(identifierString) != Lexer::KEYWORDS.end()) {
        return Token(TokenType::KEYWORD, pos, identifierString);
    }
    else {
        return Token(TokenType::IDENTIFIER, pos, identifierString);
    }
}

Token Lexer::makeEqualsToken(char character) const {
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    const char peekChar = this->positionHandler.peek();
    if (peekChar == '=') {
        positionHandler.advanceCharacter();
        return Token(TokenType::TRUEEQUALS, pos);
    }
    else {
        return Token(TokenType::EQUALS, pos);
    }
}

Token Lexer::makeNotEqualsToken(char character) const {
    std::map<std::string, std::string> pos = positionHandler.getPos();
    if (const char peekChar = this->positionHandler.peek(); peekChar == '=') {
        positionHandler.advanceCharacter();
        return Token(TokenType::NOTEQUAL, pos);
    }
    else {
        throw ExpectedCharError("\nOn line: " + std::to_string(stoi(pos["line"]) + 1) +
        " of file: " + pos["name"] + "\n" +
        "{" + pos["lineText"] + "}\n" +
        "Expected >>> != <<< instead recieved >>> !" + std::string(1, peekChar) + " <<<");
    }
}

Token Lexer::makeLessThanToken(char character) const {
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    const char peekChar = this->positionHandler.peek();
    if (peekChar == '=') {
        positionHandler.advanceCharacter();
        return Token(TokenType::LESSEQUAL, pos);
    }
    else {
        return Token(TokenType::LESSTHAN, pos);
    }
}

Token Lexer::makeGreaterThanToken(char character) const {
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    const char peekChar = this->positionHandler.peek();
    if (peekChar == '=') {
        positionHandler.advanceCharacter();
        return Token(TokenType::GREATEREQUAL, pos);
    }
    else {
        return Token(TokenType::GREATERTHAN, pos);
    }
}

std::map<int, std::vector<Token>> Lexer::tokenise() const {
    std::map<int, std::vector<Token>> tokenDict = {};
    bool isLine = positionHandler.advanceLine();

    while (isLine) { // loop through lines
        std::vector<Token> lineTokens = {};
        std::map<std::string, std::string> pos = positionHandler.getPos();
        char currentChar = positionHandler.getChar();
        bool escapeFlag = false;

        while (currentChar != '\0' and not escapeFlag) { // loop through characters
            pos = positionHandler.getPos();
            switch(currentChar){
                case ' ': case '\t': case '\n': case '\r':
                    // Skip whitespace
                    break;
                case '~':
                    escapeFlag = true;
                break;
                case '(':
                    lineTokens.emplace_back(TokenType::OPENPAREN, pos);
                break;
                case ')':
                    lineTokens.emplace_back(TokenType::CLOSEPAREN, pos);
                    break;
                case '{':
                    lineTokens.emplace_back(TokenType::OPENBRACE, pos);
                break;
                case '}':
                    lineTokens.emplace_back(TokenType::CLOSEBRACE, pos);
                break;
                case ',':
                    lineTokens.emplace_back(TokenType::SEPERATOR, pos);
                break;
                case '=':
                    lineTokens.push_back(makeEqualsToken(currentChar));
                break;
                case '!':
                    lineTokens.push_back(makeNotEqualsToken(currentChar));
                break;
                case '<':
                    lineTokens.push_back(makeLessThanToken(currentChar));
                break;
                case '>':
                    lineTokens.push_back(makeGreaterThanToken(currentChar));
                break;
                default:
                    if (OPERATORS.find(currentChar) != std::string::npos) {
                        lineTokens.push_back(makeOperatorToken(currentChar));
                    }
                    else if (DIGITS.find(currentChar) != std::string::npos) {
                        lineTokens.push_back(makeNumberToken(currentChar));
                    }
                    else if (LETTERS.find(currentChar) != std::string::npos) {
                        lineTokens.push_back(makeIdentifierToken(currentChar));
                    }
                    else {
                        throw IllegalCharError("\nUnrecognized character >>> " + pos["character"] + " <<<" +
                            " on line: " +
                            std::to_string(stoi(pos["line"]) + 1) + " of file: " + pos["name"] +
                            " {" + pos["lineText"] + "}");
                    }
            }
            currentChar = positionHandler.advanceCharacter(); // advance to next char
        }
        lineTokens.emplace_back(TokenType::EOL, positionHandler.getPos());
        tokenDict[positionHandler.getLineNumber()] = lineTokens;
        isLine = positionHandler.advanceLine();
    }
    tokenDict[positionHandler.getLineNumber()+1].emplace_back(TokenType::EOF_, PositionHandler::nullPos);
    return tokenDict;
}
