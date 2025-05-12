#include <Error.h>
#include <gtest/gtest.h>
#include <sstream>
#include "Lexer.h"
#include "Token.h"

struct LexerInput {
    std::string input;
    TokenType expectedType;
    ValueLiteral expectedValue;
};

class LexerTest : public ::testing::TestWithParam<LexerInput> {};

INSTANTIATE_TEST_SUITE_P(AllTokenTypes,
    LexerTest,
    ::testing::Values(
        // Primitives
        LexerInput{"42", TokenType::INT, 42},
        LexerInput{"3.14", TokenType::FLOAT, 3.14f},
        LexerInput{"\"hello\"", TokenType::STRING, std::string("hello")},
        // Operators
        LexerInput{"+", TokenType::PLUS, {}},
        LexerInput{"-", TokenType::MINUS, {}},
        LexerInput{"*", TokenType::MUL, {}},
        LexerInput{"/", TokenType::DIV, {}},
        LexerInput{"%", TokenType::MOD, {}},
        LexerInput{"++", TokenType::INCREMENT, {}},
        LexerInput{"--", TokenType::DECREMENT, {}},
        // Symbols
        LexerInput{"(", TokenType::OPENPAREN, {}},
        LexerInput{")", TokenType::CLOSEPAREN, {}},
        LexerInput{"{", TokenType::OPENBRACE, {}},
        LexerInput{"}", TokenType::CLOSEBRACE, {}},
        LexerInput{",", TokenType::SEPERATOR, {}},
        // Keywords and identifiers
        LexerInput{"var", TokenType::KEYWORD, std::string("var")},
        LexerInput{"if", TokenType::KEYWORD, std::string("if")},
        LexerInput{"else", TokenType::KEYWORD, std::string("else")},
        LexerInput{"while", TokenType::KEYWORD, std::string("while")},
        LexerInput{"for", TokenType::KEYWORD, std::string("for")},
        LexerInput{"func", TokenType::KEYWORD, std::string("func")},
        LexerInput{"return", TokenType::KEYWORD, std::string("return")},
        LexerInput{"not", TokenType::KEYWORD, std::string("not")},
        LexerInput{"and", TokenType::KEYWORD, std::string("and")},
        LexerInput{"or", TokenType::KEYWORD, std::string("or")},
        LexerInput{"out", TokenType::KEYWORD, std::string("out")},
        LexerInput{"x", TokenType::IDENTIFIER, std::string("x")},
        // Comparators and assignment
        LexerInput{"=", TokenType::EQUALS, {}},
        LexerInput{"==", TokenType::TRUEEQUALS, {}},
        LexerInput{"!=", TokenType::NOTEQUAL, {}},
        LexerInput{"<", TokenType::LESSTHAN, {}},
        LexerInput{">", TokenType::GREATERTHAN, {}},
        LexerInput{"<=", TokenType::LESSEQUAL, {}},
        LexerInput{">=", TokenType::GREATEREQUAL, {}}
    )
);

TEST_P(LexerTest, ProducesExpectedTokenType) {
    const LexerInput& param = GetParam();

    std::istringstream stream(param.input);
    PositionHandler ph("mock.vis", stream);
    Lexer lexer(ph);

    auto tokens = lexer.tokenise();
    const auto& line = tokens.begin()->second;

    ASSERT_FALSE(line.empty());

    const Token& token = line[0];
    EXPECT_EQ(token.getType(), param.expectedType) << "Input: " << param.input;

    const auto& actualValue = token.getValue();
    const auto& expectedValue = param.expectedValue;

    if (!std::holds_alternative<std::monostate>(expectedValue)) {
        ASSERT_EQ(actualValue.index(), expectedValue.index()) << "Mismatched ValueLiteral type for input: " << param.input;

        if (std::holds_alternative<int>(expectedValue)) {
            EXPECT_EQ(std::get<int>(actualValue), std::get<int>(expectedValue));
        } else if (std::holds_alternative<float>(expectedValue)) {
            EXPECT_FLOAT_EQ(std::get<float>(actualValue), std::get<float>(expectedValue));
        } else if (std::holds_alternative<std::string>(expectedValue)) {
            EXPECT_EQ(std::get<std::string>(actualValue), std::get<std::string>(expectedValue));
        } else if (std::holds_alternative<bool>(expectedValue)) {
            EXPECT_EQ(std::get<bool>(actualValue), std::get<bool>(expectedValue));
        }
    }
}

TEST(LexerTest, TokenizeCommentedLine) {
    std::istringstream stream("x - 7 ~ this is a comment");
    PositionHandler ph("mock.vis", stream);
    const Lexer lexer = Lexer(ph);
    auto tokens = lexer.tokenise();
    ASSERT_EQ(tokens.size(), 2);
    const auto& line = tokens.at(0);
    ASSERT_EQ(line.size(), 4);

    EXPECT_EQ(line[0].getType(), TokenType::IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(line[0].getValue()), "x");

    EXPECT_EQ(line[1].getType(), TokenType::MINUS);

    EXPECT_EQ(line[2].getType(), TokenType::INT);
    EXPECT_EQ(std::get<int>(line[2].getValue()), 7);

    EXPECT_EQ(line[3].getType(), TokenType::EOL);
}

TEST(LexerTest, MakeNumberTokenFail) {
    std::istringstream stream("5.5.5");
    PositionHandler ph("mock.vis", stream);
    const Lexer lexer = Lexer(ph);
    EXPECT_THROW(lexer.tokenise(), IllegalCharError);
}

TEST(LexerTest, MakeStringTokenFail) {
    std::istringstream stream("\"testString");
    PositionHandler ph("mock.vis", stream);
    const Lexer lexer = Lexer(ph);
    EXPECT_THROW(lexer.tokenise(), InvalidSyntaxError);
}

TEST(LexerTest, MakeNotEqualsTokenFail) {
    std::istringstream stream("!");
    PositionHandler ph("mock.vis", stream);
    const Lexer lexer = Lexer(ph);
    EXPECT_THROW(lexer.tokenise(), ExpectedCharError);
}

TEST(LexerTest, TokenizeUnrecognisedChar) {
    std::istringstream stream("|");
    PositionHandler ph("mock.vis", stream);
    const Lexer lexer = Lexer(ph);
    EXPECT_THROW(lexer.tokenise(), IllegalCharError);

}


