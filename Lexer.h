#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "Token.h"
#include "PositionHandler.h"

class Lexer {
    public:
        // Constructor
        Lexer(PositionHandler& positionHandler_);

        // Tokenize a line of input and return a vector of Tokens
        std::vector<Token> tokenise();

    private:
        PositionHandler& positionHandler;
        const std::string DIGITS = "0123456789";

        // Private helper method to tokenize numbers
        Token makeNumberToken();
};

#endif // LEXER_H