//
// Created by joshu on 02/02/2025.
//

#include "Parser.h"

#include <sstream>
#include <memory>
#include <PositionHandler.h>
#include <utility>

#include "Lexer.h"
#include "Token.h"


Parser::Parser(std::map<int, std::vector<Token>> tokenizedFile):
lineIndex(-1),
tokenIndex(-1),
tokenDict(std::move(tokenizedFile)),
tokenVector(tokenizedFile[lineIndex]),
currentToken(nullptr) {
    advanceLine();
}

bool Parser::advanceLine() { // returns true if advanced
    if (lineIndex+1 > tokenDict.rbegin()->first) {
        return false;
    }
    lineIndex++;
    tokenIndex = -1;
    tokenVector = tokenDict[lineIndex];
    advanceToken();
    return true;
}

Token* Parser::advanceToken() {
    tokenIndex++;
    if ( tokenIndex < tokenVector.size()) {
        currentToken = &tokenVector[tokenIndex];
    }
    return currentToken;
}

std::unique_ptr<Node> Parser::parse() {
    std::unique_ptr<Node> returnNode = nullptr;
    if (!tokenVector.empty()) {
        if (currentToken->getType() == TokenType::EOF_) {
            returnNode = std::make_unique<EndOfFile>(*currentToken);
        }
        else if (currentToken->matches(TokenType::KEYWORD, "func")) {
            returnNode = funcDef();
        }
        else {
            returnNode = statement();
        }
        if (currentToken->getType() == TokenType::EOL) {
            advanceLine();
        }
    }
    return returnNode;
}

InvalidSyntaxError Parser::makeSyntaxError(std::map<std::string, std::string> position,
                                            const std::string &expectedType) {
    std::ostringstream oss;
    oss << "\nError in file: " << position["name"]
    << "\n>>> line: " << std::to_string(stoi(position["line"]) + 1)
    << " | " << position["lineText"] << "<<<"
    << "\nexpected >>> " << expectedType << " <<< "
    << "instead recieved >>> " << PositionHandler::getWordFromLine(position) << " <<<";
    return InvalidSyntaxError(oss.str());
}

std::unique_ptr<Node> Parser::binaryOperation(  const std::function<std::unique_ptr<Node>()> &func,
                                                const std::vector<TokenType> &tokenTypes,
                                                const std::vector<std::string> &tokenValues) {

    const bool containsKeyword = std::find( tokenTypes.begin(),tokenTypes.end(),TokenType::KEYWORD) != tokenTypes.end();
    if (containsKeyword && tokenValues.empty()) {
        throw ParseError("binaryOperation was called with TokenType::KEYWORD but no tokenValues were provided");
    }
    if (not containsKeyword && not tokenValues.empty()) {
        throw ParseError("binaryOperation was called without TokenType::KEYWORD but tokenValues were provided");
    }

    std::unique_ptr<Node> left = func();

    while (true) {
        bool matchFound = false;
        if (containsKeyword) {
            for (const std::string& value : tokenValues) {
                if(currentToken->matches(TokenType::KEYWORD, value)) {
                    matchFound = true;
                    break;
                }
            }
        }
        else {
            matchFound = std::find(tokenTypes.begin(), tokenTypes.end(), currentToken->getType()) != tokenTypes.end();
        }

        if (not matchFound) {// exit loop if no TokenTypes were correct
            break;
        }

        const Token* opToken = currentToken;
        advanceToken();
        std::unique_ptr<Node> right = func();
        left = std::make_unique<BinaryOperator>(std::move(left), Operator(*opToken), std::move(right));
    }
    return left;
}

std::unique_ptr<Node> Parser::funcDef() {
    advanceToken();
    if (currentToken->getType() != TokenType::IDENTIFIER) {throw makeSyntaxError(currentToken->getPos(), "IDENTIFIER");}
    Token identifierToken = *currentToken;
    advanceToken();
    if (currentToken->getType() != TokenType::OPENPAREN) {throw makeSyntaxError(currentToken->getPos(), "(");}
    advanceToken();
    std::vector<Token> funcArgTokens = {};
    do {
        if (currentToken->getType() == TokenType::CLOSEPAREN) {break;}
        else if (currentToken->getType() == TokenType::IDENTIFIER) {funcArgTokens.push_back(currentToken->clone());}
        else {throw InvalidSyntaxError(
            "Function >>> " + std::get<std::string>(identifierToken.getValue()) + " <<< "
            + "expected argument of type IDENTIFIER instead recieved: "
            + tokenTypeToStr(currentToken->getType())
            );
        }
        advanceToken();
        if (currentToken->getType() == TokenType::CLOSEPAREN) {break;}
        else if (currentToken->getType() == TokenType::SEPERATOR){advanceToken();}
        else {throw makeSyntaxError(currentToken->getPos(), ", OR )");}
    }
    while (true);
    advanceToken();
    while (currentToken->getType() == TokenType::EOL) {advanceLine();}
    if (currentToken->getType() != TokenType::OPENBRACE) {throw makeSyntaxError(currentToken->getPos(), "{");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    advanceLine();
    std::vector<std::unique_ptr<Node>> funcNodes = {};
    bool lineCheck = false;
    do {
        if (std::unique_ptr<Node> node = parse()) {
            lineCheck = true;
            funcNodes.push_back(std::move(node));
        }
    }
    while (currentToken->getType() != TokenType::CLOSEBRACE);
    if (not lineCheck) {throw InvalidSyntaxError("cannot define function with no statements");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    return std::make_unique<FuncDef>(identifierToken, std::move(funcArgTokens), std::move(funcNodes));
}

std::unique_ptr<Node> Parser::statement() {
    if (currentToken->matches(TokenType::KEYWORD, "return")) {
        return returnStmt();
    }
    else if (currentToken->matches(TokenType::KEYWORD, "while")) {
        return whileStmt();
    }
    else if (currentToken->matches(TokenType::KEYWORD, "for")) {
        return forStmt();
    }
    else if (currentToken->matches(TokenType::KEYWORD, "if")) {
        return ifStmt();
    }
    else {
        return expression();
    }
}

std::unique_ptr<Node> Parser::returnStmt() {
    Token returnToken = *currentToken;
    advanceToken();
    std::unique_ptr<Node> expressionNode =  expression();
    if (currentToken->getType() != TokenType::EOL) {
        throw InvalidSyntaxError("cannot have any thing after return statement");
    }
    return std::make_unique<ReturnCall>(returnToken, std::move(expressionNode));
}

std::unique_ptr<Node> Parser::whileStmt() {
    advanceToken();
    if (currentToken->getType() != TokenType::OPENPAREN) {throw makeSyntaxError(currentToken->getPos(), "(");}
    advanceToken();
    std::unique_ptr<Node> condition = this->expression();
    if (currentToken->getType() != TokenType::CLOSEPAREN) {throw makeSyntaxError(currentToken->getPos(), ")");}
    advanceToken();
    while (currentToken->getType() == TokenType::EOL) {advanceLine();}
    if (currentToken->getType() != TokenType::OPENBRACE) {throw makeSyntaxError(currentToken->getPos(), "{");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    advanceLine();
    std::vector<std::unique_ptr<Node>> whileNodes = {};
    bool lineCheck = false;
    do {
        if (std::unique_ptr<Node> node = parse()) {
            lineCheck = true;
            whileNodes.push_back(std::move(node));
        }
    }
    while (currentToken->getType() != TokenType::CLOSEBRACE);
    if (not lineCheck) {throw InvalidSyntaxError("cannot have while statement with no contents");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    return std::make_unique<WhileStmt>(std::move(condition), std::move(whileNodes));
}

std::unique_ptr<Node> Parser::forStmt() {
    advanceToken();
    if (currentToken->getType() != TokenType::OPENPAREN) {throw makeSyntaxError(currentToken->getPos(), "(");}
    advanceToken();
    std::unique_ptr<Node> varInit= this->varExpr();
    if (currentToken->getType() != TokenType::SEPERATOR) {throw makeSyntaxError(currentToken->getPos(), ",");}
    advanceToken();
    std::unique_ptr<Node> condition = this->compExpr();
    if (currentToken->getType() != TokenType::SEPERATOR) {throw makeSyntaxError(currentToken->getPos(), ",");}
    advanceToken();
    std::unique_ptr<Node> step = this->varExpr();
    if (currentToken->getType() != TokenType::CLOSEPAREN) {throw makeSyntaxError(currentToken->getPos(), ")");}
    advanceToken();
    while (currentToken->getType() == TokenType::EOL) {advanceLine();}
    if (currentToken->getType() != TokenType::OPENBRACE) {throw makeSyntaxError(currentToken->getPos(), "{");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    advanceLine();
    std::vector<std::unique_ptr<Node>> forNodes = {};
    bool lineCheck = false;
    do {
        if (std::unique_ptr<Node> node = parse()) {
            lineCheck = true;
            forNodes.push_back(std::move(node));
        }
    }
    while (currentToken->getType() != TokenType::CLOSEBRACE);
    if (not lineCheck) {throw InvalidSyntaxError("cannot have for statement with no contents");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    return std::make_unique<ForStmt>(std::move(varInit), std::move(condition), std::move(step), std::move(forNodes));
}

std::unique_ptr<Node> Parser::ifStmt() {
    advanceToken();
    if (currentToken->getType() != TokenType::OPENPAREN) {throw makeSyntaxError(currentToken->getPos(), "(");}
    advanceToken();
    std::unique_ptr<Node> ifExpression = this->expression();
    if (currentToken->getType() != TokenType::CLOSEPAREN) {throw makeSyntaxError(currentToken->getPos(), ")");}
    advanceToken();
    while (currentToken->getType() == TokenType::EOL) {advanceLine();}
    if (currentToken->getType() != TokenType::OPENBRACE) {throw makeSyntaxError(currentToken->getPos(), "{");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
    advanceLine();
    std::vector<std::unique_ptr<Node>> ifNodes = {};
    std::vector<std::unique_ptr<Node>> elseNodes = {};
    bool lineCheck = false;
    do {
        if (std::unique_ptr<Node> node = parse()) {
            lineCheck = true;
            ifNodes.push_back(std::move(node));
        }
    }
    while (currentToken->getType() != TokenType::CLOSEBRACE);
    if (not lineCheck) {throw InvalidSyntaxError("cannot have if statement with no contents");}
    advanceToken();
    if (currentToken->getType() != TokenType::EOL) {
        if (not currentToken->matches(TokenType::KEYWORD, "else")) {
            throw makeSyntaxError(currentToken->getPos(), "else");
        }
    }
    else {while (currentToken->getType() == TokenType::EOL) {advanceLine();}}
    if (currentToken->matches(TokenType::KEYWORD, "else")) {
        advanceToken();
        while (currentToken->getType() == TokenType::EOL) {advanceLine();}
        if (currentToken->getType() != TokenType::OPENBRACE) {throw makeSyntaxError(currentToken->getPos(), "< { >");}
        advanceToken();
        if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
        advanceLine();
        lineCheck = false;
        do {
            if (currentToken->getType() == TokenType::EOF_) {throw makeSyntaxError(currentToken->getPos(), "< } >");}
            if (std::unique_ptr<Node> node = parse()) {
                lineCheck = true;
                elseNodes.push_back(std::move(node));
            }
        }
        while (currentToken->getType() != TokenType::CLOSEBRACE);
        if (not lineCheck) {throw InvalidSyntaxError("cannot have else statement with no contents");}
        advanceToken();
        if (currentToken->getType() != TokenType::EOL) {throw makeSyntaxError(currentToken->getPos(), "<nothing>");}
        advanceLine();
    }
    return std::make_unique<IfStmt>(std::move(ifExpression), std::move(ifNodes), std::move(elseNodes));
}

std::unique_ptr<Node> Parser::expression() {
    if (currentToken->getType() == TokenType::KEYWORD &&
    Lexer::LIBWORDS.find(std::get<std::string>(currentToken->getValue())) != Lexer::LIBWORDS.end()) {
        return libExpr();
    }
    else if (currentToken->matches(TokenType::KEYWORD, "var")) {
        return varExpr();
    }
    else {
        return compExpr();
    }
}

std::unique_ptr<Node> Parser::libExpr() {
    Token identifierToken = *currentToken;
    advanceToken();
    if (currentToken->getType() != TokenType::OPENPAREN) {throw makeSyntaxError(currentToken->getPos(), "(");}
    advanceToken();
    std::vector<std::unique_ptr<Node>> argumentNodes = {};
    do {
        if (currentToken->getType() == TokenType::CLOSEPAREN) {break;}
        if (std::unique_ptr<Node> expr = expression()) {argumentNodes.push_back(std::move(expr));}
        else {throw ParseError("argument in function call returned null Node");}
        if (currentToken->getType() == TokenType::CLOSEPAREN) {break;}
        else if (currentToken->getType() == TokenType::SEPERATOR){advanceToken();}
        else {throw makeSyntaxError(currentToken->getPos(), ", OR )");}
    }
    while (true);
    advanceToken();
    return std::make_unique<LibCall>(identifierToken, std::move(argumentNodes));
}

std::unique_ptr<Node> Parser::varExpr() {
    advanceToken();
    if (currentToken->getType() != TokenType::IDENTIFIER) {throw makeSyntaxError(currentToken->getPos(), "identifier");}
    const Token* varToken = currentToken;
    advanceToken();
    if (currentToken->getType() == TokenType::INCREMENT) {
        advanceToken();
        return std::make_unique<VarIncrement>(*varToken);
    }
    else if (currentToken->getType() == TokenType::DECREMENT) {
        advanceToken();
        return std::make_unique<VarDecrement>(*varToken);
    }
    else if (currentToken->getType() == TokenType::EQUALS) {
        advanceToken();
        std::unique_ptr<Node> expression = this->expression();
        return std::make_unique<VarAssignment>(*varToken, std::move(expression));
    }
    else {throw makeSyntaxError(currentToken->getPos(), "EQUALS, INCREMENT, DECREMENT");}
}

std::unique_ptr<Node> Parser::compExpr() {
    return binaryOperation([this](){return comparision();},
            std::vector<TokenType>{TokenType::KEYWORD}, std::vector<std::string>{"and", "not", "or"});
}

std::unique_ptr<Node> Parser::comparision() {
    if (currentToken->matches(TokenType::KEYWORD, "not")) {
        Token* opToken = currentToken;
        advanceToken();
        std::unique_ptr<Node> valueNode = comparision();
        return std::make_unique<UnaryOperator>(Operator(*opToken), std::move(valueNode));
    }
    else {
        return binaryOperation([this](){return arithmeticExpression();},
            std::vector{
                TokenType::TRUEEQUALS,
                TokenType::NOTEQUAL,
                TokenType::LESSTHAN,
                TokenType::GREATERTHAN,
                TokenType::LESSEQUAL,
                TokenType::GREATEREQUAL
            });
    }
}

std::unique_ptr<Node> Parser::arithmeticExpression() {
    return binaryOperation([this]() {return term();}, std::vector<TokenType>{TokenType::PLUS, TokenType::MINUS});
}

std::unique_ptr<Node> Parser::term() {
    return binaryOperation([this]() {return factor();}, std::vector<TokenType>{TokenType::MUL, TokenType::DIV, TokenType::MOD});
}

std::unique_ptr<Node> Parser::factor() {
    Token* token = currentToken;
    if (token->getType() == TokenType::PLUS or token->getType() == TokenType::MINUS) {
        advanceToken();
        std::unique_ptr<Node> valueNode = call();
        return std::make_unique<UnaryOperator>(Operator(*token), std::move(valueNode));
    }
    return call();
}

std::unique_ptr<Node> Parser::call() {
    Token identifierToken = *currentToken;
    std::unique_ptr<Node> node = atom();
    if (currentToken->getType() == TokenType::OPENPAREN) {
        advanceToken();
        std::vector<std::unique_ptr<Node>> argumentNodes = {};
        do {
            if (currentToken->getType() == TokenType::CLOSEPAREN) {break;}
            if (std::unique_ptr<Node> expr = expression()) {argumentNodes.push_back(std::move(expr));}
            else {throw ParseError("argument in function call returned null Node");}
            if (currentToken->getType() == TokenType::CLOSEPAREN) {break;}
            else if (currentToken->getType() == TokenType::SEPERATOR){advanceToken();}
            else {throw makeSyntaxError(currentToken->getPos(), ", OR )");}
        }
        while (true);
        advanceToken();
        node = std::make_unique<FuncCall>(identifierToken, std::move(argumentNodes));
    }
    return node;
}

std::unique_ptr<Node> Parser::atom() {
    Token* token = currentToken;
    if (token->getType() == TokenType::EOL) {
        advanceLine();
        return nullptr;
    }

    if (token->getType() == TokenType::INT or token->getType() == TokenType::FLOAT) {
        advanceToken();
        return std::make_unique<Number>(*token);
    }

    if (token->getType() == TokenType::STRING) {
        advanceToken();
        return std::make_unique<StringNode>(*token);
    }

    if (token->getType() == TokenType::IDENTIFIER) {
        advanceToken();
        return std::make_unique<VarAccess>(*token);
    }

    if (token->getType() == TokenType::OPENPAREN) {
        advanceToken();
        std::unique_ptr<Node> expr = expression();
        if (currentToken->getType() == TokenType::CLOSEPAREN) {
            advanceToken();
            return expr;
        }
        throw makeSyntaxError(currentToken->getPos(), "CLOSEPAREN");
    }

    throw makeSyntaxError(currentToken->getPos(), "int, float or identifier");
}

Parser::~Parser()= default;
