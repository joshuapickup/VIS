#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

// TokenType enum representing the different types of tokens
enum class TokenType {
    INT,
    FLOAT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    OPENPAREN,
    CLOSEPAREN
};

// Token class representing individual tokens with a type and optional value
class Token {
    public:
        Token(TokenType type_, const std::string& value_ = "");
        TokenType getType() const;
        std::string getValue() const;

        // Overload the << operator to easily print tokens
        friend std::ostream& operator<<(std::ostream& os, const Token& token);

    private:
        TokenType type;
        std::string value;

};

#endif // TOKEN_H