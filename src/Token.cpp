#include <iostream>
#include <utility>
#include "Token.h"

std::string tokenTypeToStr(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::INCREMENT: return "INCREMENT";
        case TokenType::DECREMENT: return "DECREMENT";
        case TokenType::OPENPAREN: return "OPENPAREN";
        case TokenType::CLOSEPAREN: return "CLOSEPAREN";
        case TokenType::OPENBRACE: return "OPENBRACE";
        case TokenType::CLOSEBRACE: return "CLOSEBRACE";
        case TokenType::SEPERATOR: return "SEPERATOR";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::NOTEQUAL: return "NOT EQUALS";
        case TokenType::TRUEEQUALS: return "TRUE EQUALS";
        case TokenType::LESSTHAN: return "LESS THAN";
        case TokenType::LESSEQUAL: return "LESS OR EQUALS";
        case TokenType::GREATERTHAN: return "GREATER THAN";
        case TokenType::GREATEREQUAL: return "GREATER OR EQUALS";
        case TokenType::NULL_: return "NULL_";
        case TokenType::EOL: return "END OF LINE";
        case TokenType::EOF_: return "END OF FILE";
        default: return "UNKNOWN";
    }
}

Token::Token(const TokenType type_, const std::map<std::string, std::string>& pos, ValueLiteral  value_):
    type(type_),
    position(pos),
    value(std::move(value_)) {}

TokenType Token::getType() const {return type;}

ValueLiteral Token::getValue() const {return value;}

std::map<std::string, std::string> Token::getPos() const {return position;}

bool Token::matches(const TokenType type_, const std::string &value_) const {
    if (std::holds_alternative<std::string>(value)) {
        if (type == type_ && std::get<std::string>(value) == value_) {
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os,  const Token& token) {
    os << "Token(Type: " << tokenTypeToStr(token.getType()) << ", ";
    os << "Position: {line: " << token.getPos()["line"] << " | Pos:" << token.getPos()["charPos"] << "}, ";
    os << "Value: ";

    if (std::holds_alternative<std::monostate>(token.getValue())) {
        os << "No value";  // Case for operators
    }
    else if (std::holds_alternative<int>(token.getValue())) {
        os << std::get<int>(token.getValue());
    }
    else if (std::holds_alternative<float>(token.getValue())) {
        os << std::get<float>(token.getValue());
    }
    else if (std::holds_alternative<std::string>(token.getValue())) {
        os << std::get<std::string>(token.getValue());
    }
    else if (std::holds_alternative<bool>(token.getValue())) {
        os << (std::get<bool>(token.getValue()) ? "true" : "false");
    } else {
        os << "Unknown Type";  // Fallback case (should never happen)
    }

    os << ")";
    return os;
}
