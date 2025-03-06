//
// Created by joshu on 02/02/2025.
//

#include <iostream>

#include "Parser.h"
#include "Token.h"
#include <sstream>
#include <memory>


Parser::Parser(std::vector<Token> tokens): tokenVector(std::move(tokens)), currentToken(Token(TokenType::NULL_)) {
    tokenIndex = -1;
    advance();
}

Token Parser::advance() {
    tokenIndex++;
    if ( tokenIndex < tokenVector.size()) {
        currentToken = tokenVector[tokenIndex];
    }
    return currentToken;
}

InvalidSyntaxError Parser::makeSyntaxError(const std::string &expectedType) const {
    std::ostringstream oss;
    oss << "expected " << expectedType << " instead received: " << tokenTypeToStr(currentToken.getType());
    return InvalidSyntaxError(oss.str());
}

std::unique_ptr<Node> Parser::parse() {
    if (tokenVector.empty()) {
        return nullptr;
    }
    else {
        return expression();
    }
}

std::unique_ptr<Node> Parser::binaryOperation(const std::function<std::unique_ptr<Node>()> &func, const std::array<TokenType,2> &tokenTypes) {
    std::unique_ptr<Node> left = func();
    while(currentToken.getType() == tokenTypes[0] or currentToken.getType() == tokenTypes[1]) {
        Token opToken = currentToken;
        advance();
        std::unique_ptr<Node> right = func();
        std::unique_ptr<Node> tempPtr = std::make_unique<BinaryOperator>(std::move(left), Operator(opToken), std::move(right));
        left = std::move(tempPtr);
    }
    return left;
}

std::unique_ptr<Node> Parser::expression() {
    if(currentToken.matches(TokenType::KEYWORD, "var")) {
        advance();
        if (currentToken.getType() != TokenType::IDENTIFIER) {throw makeSyntaxError("identifier");}
        const Token varToken = currentToken;
        advance();
        if (currentToken.getType() != TokenType::EQUALS) {throw makeSyntaxError("equals");}
        advance();
        std::unique_ptr<Node> expression = this->expression();
        return std::make_unique<VarAssignment>(varToken, std::move(expression));
    }
    else {
        return binaryOperation([this](){return term();}, std::array<TokenType,2>{TokenType::PLUS, TokenType::MINUS});
    }
}

std::unique_ptr<Node> Parser::term() {
    return binaryOperation([this]() {return factor();}, std::array<TokenType,2>{TokenType::MUL, TokenType::DIV});
}

std::unique_ptr<Node> Parser::factor() {
    Token token = currentToken;
    if (token.getType() == TokenType::PLUS or token.getType() == TokenType::MINUS) {
        advance();
        Token nextToken = currentToken;
        if (nextToken.getType() == TokenType::INT or nextToken.getType() == TokenType::FLOAT) {
            advance();
            return std::make_unique<UnaryOperator>(Operator(token), std::make_unique<Number>(nextToken));
        }
        else{
            throw makeSyntaxError("int or float");
        }
    }
    return atom();
}

std::unique_ptr<Node> Parser::atom() {
    Token token = currentToken;
    if (token.getType() == TokenType::INT or token.getType() == TokenType::FLOAT) {
        advance();
        return std::make_unique<Number>(token);
    }

    if (token.getType() == TokenType::IDENTIFIER) {
        advance();
        return std::make_unique<VarAccess>(token);
    }

    if (token.getType() == TokenType::OPENPAREN) {
        advance();
        std::unique_ptr<Node> expr = expression();
        if (currentToken.getType() == TokenType::CLOSEPAREN) {
            advance();
            return expr;
        }
        throw makeSyntaxError("CLOSEPAREN");
    }

    throw makeSyntaxError("int, float or identifier");
}

Parser::~Parser()= default;
