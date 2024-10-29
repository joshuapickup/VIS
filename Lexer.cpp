#include "Lexer.h"
#include "Error.h"

// Constructor
Lexer::Lexer(PositionHandler& positionHandler_) : positionHandler(positionHandler_) {}

// Helper method to create a number token (INT or FLOAT)
Token Lexer::makeNumberToken() {
    std::string numberStr;
    bool dotCheck = false;

    // Loop until the end of line or non-digit character
    while (positionHandler.getChar() != '\n' &&
           (isdigit(positionHandler.getChar()) || positionHandler.getChar() == '.')) {
        if (positionHandler.getChar() == '.') {
            if (!dotCheck) {
                dotCheck = true;
            } else {
                throw IllegalCharError("Cannot have a number with two dots");
            }
        }
        numberStr += positionHandler.getChar();
        positionHandler.advanceCharacter();
    }

    // Return either an INT or FLOAT token based on the presence of a dot
    if (dotCheck) {
        return Token(TokenType::FLOAT, numberStr);
    } else {
        return Token(TokenType::INT, numberStr);
    }
}

// Method to tokenize a line of input
std::vector<Token> Lexer::tokenise() {
    std::vector<Token> tokens;

    while (positionHandler.getChar() != '\n') {
        char currentChar = positionHandler.getChar();

        if (isspace(currentChar)) {
            // Skip whitespace
        } else if (isdigit(currentChar)) {
            tokens.push_back(makeNumberToken());
            continue;
        } else if (currentChar == '+') {
            tokens.emplace_back(TokenType::PLUS);
        } else if (currentChar == '-') {
            tokens.emplace_back(TokenType::MINUS);
        } else if (currentChar == '*') {
            tokens.emplace_back(TokenType::MUL);
        } else if (currentChar == '/') {
            tokens.emplace_back(TokenType::DIV);
        } else {
            // Unrecognized character, throw an error with position information
            auto pos = positionHandler.getPos();
            throw IllegalCharError("Unrecognized character in file " + pos["name"] +
                                   " on line: " + std::to_string(stoi(pos["line"]) + 1) +
                                   ", position: " + pos["charPos"] +
                                   "\n character: (" + pos["character"] +
                                   ") in line " + pos["lineText"]);
        }

        // Move to the next character
        positionHandler.advanceCharacter();
    }

    return tokens;
}