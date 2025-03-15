#include "Lexer.h"
#include "Error.h"


const std::unordered_set<std::string> Lexer::KEYWORDS = {"var","and","or", "not"};
const std::string Lexer::DIGITS = "0123456789";
const std::string Lexer::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
const std::string Lexer::LETTERS_DIGITS = Lexer::LETTERS + Lexer::DIGITS;
const std::string Lexer::OPERATORS = "+-*/";
const char Lexer::COMMENT = '~';

Lexer::Lexer(PositionHandler& positionHandler) : positionHandler(positionHandler) {}

Token Lexer::makeOperatorToken(const char character) const {
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    switch(character) {
        case '+': return Token(TokenType::PLUS, pos);
        case '-': return Token(TokenType::MINUS, pos);
        case '*': return Token(TokenType::MUL, pos);
        case '/': return Token(TokenType::DIV, pos);
        case '=': return Token(TokenType::EQUALS, pos);
        default: throw LexerError("operator token cant be created no case defined for character >>" +
                                            std::string(1, character) + "<<");
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
            if (!dotFlag) {dotFlag = true;}
            else {throw IllegalCharError("cannot define number with two decimals");}
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
    char peekChar = character;
    while (peekChar != '\0' and Lexer::LETTERS_DIGITS.find(peekChar) != std::string::npos) {
        character = this->positionHandler.advanceCharacter();
        identifierString += character;
        peekChar = this->positionHandler.peek();
    }

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
    const std::map<std::string, std::string> pos = positionHandler.getPos();
    const char peekChar = this->positionHandler.peek();
    if (peekChar == '=') {
        positionHandler.advanceCharacter();
        return Token(TokenType::NOTEQUAL, pos);
    }
    else {
        throw ExpectedCharError("expected = intead recieved: <" + std::string(1, peekChar) + ">");
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

std::vector<Token> Lexer::tokenise() const {
    std::vector<Token> tokens;
    char currentChar = positionHandler.getChar();
    while (currentChar != '\0') {
        std::map<std::string, std::string> pos = positionHandler.getPos();
        switch(currentChar){
            case ' ': case '\t': case '\n': case '\r':
                // Skip whitespace
                break;
            case '~':
                return tokens;
            case '(':
                tokens.emplace_back(TokenType::OPENPAREN, pos);
                break;
            case ')':
                tokens.emplace_back(TokenType::CLOSEPAREN, pos);
                break;
            case '=':
                tokens.push_back(makeEqualsToken(currentChar));
                break;
            case '!':
                tokens.push_back(makeNotEqualsToken(currentChar));
                break;
            case '<':
                tokens.push_back(makeLessThanToken(currentChar));
                break;
            case '>':
                tokens.push_back(makeGreaterThanToken(currentChar));
                break;
            default:
                if (OPERATORS.find(currentChar) != std::string::npos) {
                    tokens.push_back(makeOperatorToken(currentChar));
                }
                else if (DIGITS.find(currentChar) != std::string::npos) {
                    tokens.push_back(makeNumberToken(currentChar));
                }
                else if (LETTERS.find(currentChar) != std::string::npos) {
                    tokens.push_back(makeIdentifierToken(currentChar));
                }
                else {
                    throw IllegalCharError("\nUnrecognized character in file " + pos["name"] + "\n"
                                           "on line: " + std::to_string(stoi(pos["line"]) + 1) + "\n"
                                           "position: " + pos["charPos"] + "\n"
                                           "character: (" + pos["character"] + ")\n"
                                           "in line " + pos["lineText"]);
                }
        }
        currentChar = positionHandler.advanceCharacter(); // advance to next char
    }
    return tokens;
}
