#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <unordered_set>
#include <string>
#include "PositionHandler.h"
#include "Token.h"


// lexer class will tokenize a given string
class Lexer {
    public:
        explicit Lexer(PositionHandler& positionHandler);
        [[nodiscard]] std::vector<Token> tokenise() const;
        ~Lexer() = default;
    private:
        static const std::unordered_set<std::string> KEYWORDS;
        static const std::string DIGITS;
        static const std::string LETTERS;
        static const std::string LETTERS_DIGITS;
        static const std::string OPERATORS;
        static const char COMMENT;
        [[nodiscard]] Token makeNumberToken(char character) const;
        [[nodiscard]] Token makeIdentifierToken(char character) const;
        [[nodiscard]] static Token makeOperatorToken(char character);
        PositionHandler& positionHandler;

};

#endif // LEXER_H