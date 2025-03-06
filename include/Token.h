#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// defined token types
enum class TokenType {
    NULL_,
    INT,
    FLOAT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    OPENPAREN,
    CLOSEPAREN,
    KEYWORD,
    IDENTIFIER,
    EQUALS,
};

struct ValueLiteral {
    bool boolVal;
    int intVal;
    float floatVal;
    std::string stringVal;
};

std::string tokenTypeToStr(TokenType type);

// token class representing individual token
class Token {
public:
    explicit Token(TokenType type_, ValueLiteral value_ = ValueLiteral());
    [[nodiscard]] TokenType getType() const;
    [[nodiscard]] ValueLiteral getValue() const;
    [[nodiscard]] bool matches(TokenType type_, const std::string &value_) const;

    // overload the << operator to easily print tokens
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
private:
    TokenType type;
    ValueLiteral value;
};


#endif //TOKEN_H
