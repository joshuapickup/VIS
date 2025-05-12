
#ifndef THESTHELPERS_H
#define THESTHELPERS_H

#include "Token.h"
#include "Literal.h"
#include "Context.h"
#include <map>
#include <string>

const std::map<std::string, std::string> dummyPos = { {"line", "0"}, {"charPos", "0"} };

static const std::vector<TokenType> allTokenTypes = {
    TokenType::NULL_,
    TokenType::EOL,
    TokenType::EOF_,
    TokenType::INT,
    TokenType::FLOAT,
    TokenType::STRING,
    TokenType::PLUS,
    TokenType::MINUS,
    TokenType::MUL,
    TokenType::DIV,
    TokenType::MOD,
    TokenType::INCREMENT,
    TokenType::DECREMENT,
    TokenType::OPENPAREN,
    TokenType::CLOSEPAREN,
    TokenType::OPENBRACE,
    TokenType::CLOSEBRACE,
    TokenType::SEPERATOR,
    TokenType::KEYWORD,
    TokenType::IDENTIFIER,
    TokenType::EQUALS,
    TokenType::TRUEEQUALS,
    TokenType::NOTEQUAL,
    TokenType::LESSTHAN,
    TokenType::GREATERTHAN,
    TokenType::LESSEQUAL,
    TokenType::GREATEREQUAL
};

inline Context makeMockContext() {
    SymbolTable global;
    global.set("null", std::make_unique<BoolLiteral>(false));
    global.set("true", std::make_unique<BoolLiteral>(true));
    global.set("false", std::make_unique<BoolLiteral>(false));
    Context ctx("mocked");
    ctx.setSymbolTable(std::move(global));
    return ctx;
}

inline std::unique_ptr<Node> makeNumbernode(int number) {
    return std::make_unique<Number>(Token(TokenType::INT, dummyPos, number));
}

#endif //THESTHELPERS_H
