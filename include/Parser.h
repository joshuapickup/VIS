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
    explicit Parser(std::map<int, std::vector<Token>> tokenizedFile);
    std::unique_ptr<Node> parse();
    ~Parser();
private:
    int lineIndex;
    int tokenIndex;
    std::map<int, std::vector<Token>> tokenDict;
    std::vector<Token> tokenVector;
    Token* currentToken;
    bool advanceLine();
    Token* advanceToken();
    [[nodiscard]] static InvalidSyntaxError makeSyntaxError(std::map<std::string, std::string> position,
                                                            const std::string &expectedType);
    std::unique_ptr<Node> binaryOperation(  const std::function<std::unique_ptr<Node>()> &func,
                                            const std::vector<TokenType> &tokenTypes,
                                            const std::vector<std::string> &tokenValues = {});
    std::unique_ptr<Node> funcDef();
    std::unique_ptr<Node> statement();
    std::unique_ptr<Node> whileStmt();
    std::unique_ptr<Node> forStmt();
    std::unique_ptr<Node> ifStmt();
    std::unique_ptr<Node> expression();
    std::unique_ptr<Node> varExpr();
    std::unique_ptr<Node> compExpr();
    std::unique_ptr<Node> comparision();
    std::unique_ptr<Node> arithmeticExpression();
    std::unique_ptr<Node> term();
    std::unique_ptr<Node> factor();
    std::unique_ptr<Node> call();
    std::unique_ptr<Node> atom();
};

#endif //PARSER_H
