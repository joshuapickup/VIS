#include <gtest/gtest.h>
#include "Token.h"
#include "TestHelpers.h"

TEST(TokenTest, CreatesIntTokenCorrectly) {
    Token token(TokenType::INT, dummyPos, 42);
    EXPECT_EQ(token.getType(), TokenType::INT);
    EXPECT_EQ(std::get<int>(token.getValue()), 42);
    EXPECT_EQ(token.getPos()["line"], "0");
    EXPECT_EQ(token.getPos()["charPos"], "0");
}

TEST(TokenUtilTest, TokenTypeToStrExpected) {
    for (TokenType type : allTokenTypes) {
        std::string str = tokenTypeToStr(type);
        EXPECT_NE(str, "UNKNOWN") << "tokenTypeToStr returned UNKNOWN for a valid TokenType: "
                                  << static_cast<int>(type);
    }
}

TEST(TokenUtilTest, TokenTypeToStrInvalid) {
    TokenType invalid = static_cast<TokenType>(999);
    std::string str = tokenTypeToStr(invalid);
    EXPECT_EQ(str, "UNKNOWN");
}

TEST(TokenTest, CloneProducesEqualToken) {
    Token original(TokenType::INT, dummyPos, 123);
    Token cloned = original.clone();
    EXPECT_EQ(cloned.getType(), original.getType());
    EXPECT_EQ(std::get<int>(cloned.getValue()), std::get<int>(original.getValue()));
    EXPECT_EQ(cloned.getPos(), original.getPos());
}

TEST(TokenTest, canOutputFloatToken) {
    Token token(TokenType::FLOAT, dummyPos, 3.14f);
    std::ostringstream oss;
    oss << token;
    std::string output = oss.str();
    EXPECT_NE(output.find("Token(Type: FLOAT"), std::string::npos);
    EXPECT_NE(output.find("line: 0"), std::string::npos);
    EXPECT_NE(output.find("Pos: 0"), std::string::npos);
    EXPECT_NE(output.find("3.14"), std::string::npos);
}

TEST(TokenTest, canOutputIntToken) {
    Token token(TokenType::INT, dummyPos, 3);
    std::ostringstream oss;
    oss << token;
    std::string output = oss.str();
    EXPECT_NE(output.find("Token(Type: INT"), std::string::npos);
    EXPECT_NE(output.find("line: 0"), std::string::npos);
    EXPECT_NE(output.find("Pos: 0"), std::string::npos);
    EXPECT_NE(output.find("3"), std::string::npos);
}

TEST(TokenTest, canOutputStringToken) {
    Token token(TokenType::STRING, dummyPos, "hello");
    std::ostringstream oss;
    oss << token;
    std::string output = oss.str();
    EXPECT_NE(output.find("Token(Type: STRING"), std::string::npos);
    EXPECT_NE(output.find("line: 0"), std::string::npos);
    EXPECT_NE(output.find("Pos: 0"), std::string::npos);
    EXPECT_NE(output.find("hello"), std::string::npos);
}

TEST(TokenTest, canOutputOperatorToken) {
    Token token(TokenType::PLUS, dummyPos);
    std::ostringstream oss;
    oss << token;
    std::string output = oss.str();
    EXPECT_NE(output.find("Token(Type: PLUS"), std::string::npos);
    EXPECT_NE(output.find("line: 0"), std::string::npos);
    EXPECT_NE(output.find("Pos: 0"), std::string::npos);
    EXPECT_NE(output.find("No value"), std::string::npos);
}

