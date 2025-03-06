#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>

// base error class, inheriting from std::runtime_error
class Error : public std::runtime_error {
public:
    explicit Error(const std::string& message);
    [[nodiscard]] virtual std::string getMessage() const;
};

// IllegalCharError class for Lexer
class IllegalCharError final : public Error {
public:
    explicit IllegalCharError(const std::string& message);
};

class LexerError final : public Error {
public:
    explicit LexerError(const std::string& message);
};

class InvalidSyntaxError final : public Error {
public:
    explicit InvalidSyntaxError(const std::string& message);
};

class VisRunTimeError final : public Error {
    public:
    explicit VisRunTimeError(const std::string& message);
};
#endif // ERROR_H
