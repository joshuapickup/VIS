#include "error.h"

// Definition of the Error constructor
Error::Error(const std::string& message): std::runtime_error(message) {
}

// Definition of the getMessage method
std::string Error::getMessage() const {
    return what();
}

// Definition of the IllegalCharError constructor
IllegalCharError::IllegalCharError(const std::string& message): Error("Illegal Character Error: " + message) {
}

LexerError::LexerError(const std::string& message): Error("Lexer Error: " + message) {
}

InvalidSyntaxError::InvalidSyntaxError(const std::string& message): Error("Syntax Error: " + message) {
}

ExpectedCharError::ExpectedCharError(const std::string& message): Error("Expected Char Error: " + message) {
}

ParseError::ParseError(const std::string& message): Error("Parse Error: " + message) {
}

VisRunTimeError::VisRunTimeError(const std::string& message): Error("RunTime Error: " + message) {
}

