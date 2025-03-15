#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>
#include <variant>

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
    TRUEEQUALS,
    NOTEQUAL,
    LESSTHAN,
    GREATERTHAN,
    LESSEQUAL,
    GREATEREQUAL,
};

using ValueLiteral = std::variant<std::monostate, bool, int, float, std::string>;

std::string tokenTypeToStr(TokenType type);

// token class representing individual token
class Token {
public:
    explicit Token(TokenType type_, const std::map<std::string, std::string>& pos, ValueLiteral  value_ = std::monostate{});
    [[nodiscard]] TokenType getType() const;
    [[nodiscard]] ValueLiteral getValue() const;
    [[nodiscard]] std::map<std::string, std::string> getPos() const;
    [[nodiscard]] bool matches(TokenType type_, const std::string &value_) const;

    // overload the << operator to easily print tokens
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
private:
    TokenType type;
    ValueLiteral value;
    std::map<std::string, std::string> position;
};


#endif //TOKEN_H
