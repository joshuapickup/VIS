#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <functional>
#include <memory>

#include "Token.h"
#include "Node.h"
#include "error.h"

class Parser {
public:
    explicit Parser(std::vector<Token>);
    std::unique_ptr<Node> parse();
    ~Parser();
private:
    std::vector<Token> tokenVector;
    Token* currentToken;
    int tokenIndex;
    [[nodiscard]] InvalidSyntaxError makeSyntaxError(const std::string &expectedType) const;
    Token* advance();
    std::unique_ptr<Node> binaryOperation(const std::function<std::unique_ptr<Node>()> &func, const std::array<TokenType,2> &tokenTypes);
    std::unique_ptr<Node> expression();
    std::unique_ptr<Node> term();
    std::unique_ptr<Node> factor();
    std::unique_ptr<Node> atom();
};



#endif //PARSER_H
