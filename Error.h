#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>

// Base Error class, inheriting from std::runtime_error
class Error : public std::runtime_error {
public:
    explicit Error(const std::string& message)
        : std::runtime_error(message) {}

    virtual std::string getMessage() const {
        return what();
    }
};

// IllegalCharError class to handle illegal character errors
class IllegalCharError : public Error {
public:
    IllegalCharError(const std::string& message)
        : Error("Illegal Character Error: " + message) {}
};

#endif // ERROR_H
