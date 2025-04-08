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
    static const std::unordered_set<std::string> LIBWORDS;
    static const std::unordered_set<std::string> KEYWORDS;
    explicit Lexer(PositionHandler& positionHandler);
    [[nodiscard]] std::map<int, std::vector<Token>> tokenise() const;
    ~Lexer() = default;
private:
    static const std::string DIGITS;
    static const std::string LETTERS;
    static const std::string LETTERS_DIGITS;
    static const std::string OPERATORS;
    static const char COMMENT;
    [[nodiscard]] Token makeNumberToken(char character) const;
    [[nodiscard]] Token makeStringToken(char character) const;
    [[nodiscard]] Token makeIdentifierToken(char character) const;
    [[nodiscard]] Token makeOperatorToken(char character) const;
    [[nodiscard]] Token makeEqualsToken(char character) const;
    [[nodiscard]] Token makeNotEqualsToken(char character) const;
    [[nodiscard]] Token makeLessThanToken(char character) const;
    [[nodiscard]] Token makeGreaterThanToken(char character) const;
    PositionHandler& positionHandler;
};

#endif // LEXER_H