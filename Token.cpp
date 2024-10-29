#include "Token.h"

// Constructor implementation
Token::Token(TokenType type_, const std::string& value_) : type(type_), value(value_) {

}

// Getter for the token type
TokenType Token::getType() const {
    return type;
}

// Getter for the token value
std::string Token::getValue() const {
    return value;
}

// Overload the << operator to print the token
std::ostream& operator<<(std::ostream& os, const Token& token) {
    // Print the token type and value, if any
    if (!token.value.empty())
        os << static_cast<int>(token.type) << ": " << token.value;
    else
        os << static_cast<int>(token.type);
    return os;
}