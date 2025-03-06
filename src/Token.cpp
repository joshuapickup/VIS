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
        case TokenType::OPENPAREN: return "OPENPAREN";
        case TokenType::CLOSEPAREN: return "CLOSEPAREN";
        case TokenType::KEYWORD: return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::NULL_: return "NULL_";
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
    if (type == type_ && value.stringVal == value_) {return true;}
    else{return false;}
}

std::ostream& operator<<(std::ostream& os,  const Token& token) {
    os << "Token(Type: " << tokenTypeToStr(token.getType()) << ", ";
    os << "Position: {line: " << token.getPos()["line"] << "| Pos:" << token.getPos()["charPos"] << "}, ";
    // show value based on TokenType
    switch (token.getType()) {
        case TokenType::INT:
            os << "Value: " << token.getValue().intVal;
        break;
        case TokenType::FLOAT:
            os << "Value: " << token.getValue().floatVal;
        break;
        case TokenType::IDENTIFIER:
        case TokenType::KEYWORD:
            os << "Value: " << token.getValue().stringVal;
        break;
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::OPENPAREN:
        case TokenType::CLOSEPAREN:
        case TokenType::EQUALS:
            os << "No value";  // Operators usually don't have a value
        break;
        default:
            os << "Unknown type";
        break;
    }
    os << ")";
    return os;
}
