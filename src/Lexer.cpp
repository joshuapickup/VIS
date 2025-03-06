#include "Lexer.h"
#include "Error.h"


const std::unordered_set<std::string> Lexer::KEYWORDS = {"var",};
const std::string Lexer::DIGITS = "0123456789";
const std::string Lexer::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
const std::string Lexer::LETTERS_DIGITS = Lexer::LETTERS + Lexer::DIGITS;
const std::string Lexer::OPERATORS = "+-*/=";
const char Lexer::COMMENT = '~';

Lexer::Lexer(PositionHandler& positionHandler) : positionHandler(positionHandler) {}

Token Lexer::makeOperatorToken(const char character) {
    switch(character) {
        case '+': return Token(TokenType::PLUS);
        case '-': return Token(TokenType::MINUS);
        case '*': return Token(TokenType::MUL);
        case '/': return Token(TokenType::DIV);
        case '=': return Token(TokenType::EQUALS);
        default: throw LexerError("operator token cant be created no case defined for character >>" +
                                            std::string(1, character) + "<<");
    }
}

Token Lexer::makeNumberToken (char character) const{// loop through line until next char isnt digit
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
    ValueLiteral literal = ValueLiteral();
    if (dotFlag) {
        literal.floatVal = std::stof(stringNum);
        return Token(TokenType::FLOAT, literal);
    }
    else {
        literal.intVal = std::stoi(stringNum);
        return Token(TokenType::INT, literal);
    }
}

Token Lexer::makeIdentifierToken(char character) const{
    std::string identifierString;
    identifierString += character;
    char peekChar = character;
    while (peekChar != '\0' and Lexer::LETTERS_DIGITS.find(peekChar) != std::string::npos) {
        character = this->positionHandler.advanceCharacter();
        identifierString += character;
        peekChar = this->positionHandler.peek();
    }

    ValueLiteral literal = ValueLiteral();
    literal.stringVal = identifierString;
    if (Lexer::KEYWORDS.find(identifierString) != Lexer::KEYWORDS.end()) {
        return Token(TokenType::KEYWORD, literal);
    }
    else {
        return Token(TokenType::IDENTIFIER, literal);
    }
}

std::vector<Token> Lexer::tokenise() const {
    std::vector<Token> tokens;
    char currentChar = positionHandler.getChar();
    while (currentChar != '\0') {
        switch(currentChar){
            case ' ': case '\t': case '\n': case '\r':
                // Skip whitespace
                break;
            case '~':
                return tokens;
            case '(':
                tokens.emplace_back(TokenType::OPENPAREN);
                break;
            case ')':
                tokens.emplace_back(TokenType::CLOSEPAREN);
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
                    std::unordered_map<std::string, std::string> pos = positionHandler.getPos();
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
