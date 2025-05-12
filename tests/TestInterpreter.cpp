
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include "Interpreter.h"
#include "TestHelpers.h"
#include "Parser.h"
#include "Literal.h"
#include "Context.h"


TEST(HelperFunctionsTest, testPrintTokens) {
    std::map<int, std::vector<Token>> tokenMap = {
        {0, {
            Token(TokenType::KEYWORD, dummyPos, "func"),
            Token(TokenType::IDENTIFIER, dummyPos, "myFunc")
        }},
        {1, {
            Token(TokenType::INT, dummyPos, 42)
        }}
    };
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printTokens(tokenMap);
    std::cout.rdbuf(oldCout);
    std::string output = buffer.str();
    EXPECT_NE(output.find("Line 0:"), std::string::npos);
    EXPECT_NE(output.find("func"), std::string::npos);
    EXPECT_NE(output.find("myFunc"), std::string::npos);
    EXPECT_NE(output.find("Line 1:"), std::string::npos);
    EXPECT_NE(output.find("42"), std::string::npos);
}

TEST(InterpreterTest, testInterpretFileFunction) {
    std::string filename = "temp_test.vis";
    std::string source = "var x = 10\nx + 10\n";
    std::ofstream tempFile(filename);
    ASSERT_TRUE(tempFile.is_open());
    tempFile << source;
    tempFile.close();
    EXPECT_NO_THROW({Interpreter::interpretFile(filename, false);});
    std::remove(filename.c_str());
}

TEST(InterpreterTest, testVisitNumber) {
    auto context = makeMockContext();
    const std::unique_ptr<Node> mockNode = makeNumbernode(6);
    std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* intLiteral = dynamic_cast<IntLiteral*>(result.get());
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getNumberValue(), 6);
}

TEST(InterpreterTest, testVisitString) {
    auto context = makeMockContext();
    const std::unique_ptr<Node> mockNode = std::make_unique<StringNode>(Token(TokenType::STRING, dummyPos, "testString"));
    std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* intLiteral = dynamic_cast<StringLiteral*>(result.get());
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getStringValue(), "testString");
}

TEST(InterpreterTest, testVisitBinaryOpNode) {
    auto context = makeMockContext();
    struct TestCase {
        TokenType opType;
        std::optional<std::string> keywordValue; // used when opType == KEYWORD
        int left;
        int right;
        std::variant<int, bool> expectedResult; // expected result
    };
    std::vector<TestCase> testCases = {
        {TokenType::PLUS, std::nullopt, 10, 20, 30},
        {TokenType::MINUS, std::nullopt, 20, 5, 15},
        {TokenType::MUL, std::nullopt, 3, 4, 12},
        {TokenType::DIV, std::nullopt, 20, 5, 4},
        {TokenType::MOD, std::nullopt, 20, 6, 2},
        {TokenType::TRUEEQUALS, std::nullopt, 10, 10, true},
        {TokenType::NOTEQUAL, std::nullopt, 10, 5, true},
        {TokenType::LESSTHAN, std::nullopt, 5, 10, true},
        {TokenType::LESSEQUAL, std::nullopt, 10, 10, true},
        {TokenType::GREATERTHAN, std::nullopt, 10, 5, true},
        {TokenType::GREATEREQUAL, std::nullopt, 10, 10, true},
        {TokenType::KEYWORD, "or", 0, 1, true},
        {TokenType::KEYWORD, "or", 0, 0, false},
        {TokenType::KEYWORD, "and", 1, 1, true},
        {TokenType::KEYWORD, "and", 1, 0, false}
    };
    for (const auto& testCase : testCases) {
        std::unique_ptr<Node> mockLeftNode = makeNumbernode(testCase.left);
        Token opToken = (testCase.opType == TokenType::KEYWORD)
            ? Token(TokenType::KEYWORD, dummyPos, testCase.keywordValue.value()) : Token(testCase.opType, dummyPos);
        Operator mockOperatorNode(opToken);
        std::unique_ptr<Node> mockRightNode = makeNumbernode(testCase.right);
        std::unique_ptr<Node> mockNode = std::make_unique<BinaryOperator>(
            std::move(mockLeftNode),
            mockOperatorNode,
            std::move(mockRightNode)
        );
        std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
        ASSERT_NE(result, nullptr);

        if (std::holds_alternative<int>(testCase.expectedResult)) {
            auto* intLiteral = dynamic_cast<IntLiteral*>(result.get());
            ASSERT_NE(intLiteral, nullptr) << "Expected IntLiteral for operator " << static_cast<int>(testCase.opType);
            EXPECT_EQ(intLiteral->getNumberValue(), std::get<int>(testCase.expectedResult));
        }
        else {
            auto* boolLiteral = dynamic_cast<BoolLiteral*>(result.get());
            ASSERT_NE(boolLiteral, nullptr) << "Expected BoolLiteral for operator " << static_cast<int>(testCase.opType);
            EXPECT_EQ(boolLiteral->getBoolValue(), std::get<bool>(testCase.expectedResult));
        }
    }
}

TEST(InterpreterTest, testVisitUnaryOpNode) {
    auto context = makeMockContext();

    Operator mockOperatorNode = Operator(Token(TokenType::MINUS, dummyPos));
    std::unique_ptr<Node> mockValueNode = makeNumbernode(20);

    Operator mockOperatorNode2 = Operator(Token(TokenType::KEYWORD, dummyPos, "not"));
    std::unique_ptr<Node> mockValueNode2 = makeNumbernode(20);

    const std::unique_ptr<Node> mockNode = std::make_unique<UnaryOperator>(mockOperatorNode, std::move(mockValueNode));
    const std::unique_ptr<Node> mockNode2 = std::make_unique<UnaryOperator>(mockOperatorNode2, std::move(mockValueNode2));

    const std::unique_ptr<Literal> result1 = Interpreter::visit(mockNode, &context);
    const std::unique_ptr<Literal> result2 = Interpreter::visit(mockNode2, &context);
    ASSERT_NE(result1, nullptr);
    ASSERT_NE(result2, nullptr);

    auto* intLiteral = dynamic_cast<IntLiteral*>(result1.get());
    auto* boolLiteral = dynamic_cast<BoolLiteral*>(result2.get());
    ASSERT_NE(intLiteral, nullptr);
    ASSERT_NE(boolLiteral, nullptr);
    EXPECT_EQ(intLiteral->getNumberValue(), -20);
    EXPECT_EQ(boolLiteral->getBoolValue(), false);
}

TEST(InterpreterTest, testVisitIncrementNode) {
    auto context = makeMockContext();
    context.getSymbolTable().set("x", std::make_unique<IntLiteral>(10));
    const std::unique_ptr<Node> mockNode = std::make_unique<VarIncrement>(Token(TokenType::IDENTIFIER, dummyPos, "x"));

    const std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* intLiteral = dynamic_cast<IntLiteral*>(result.get());
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getNumberValue(), 11);
}

TEST(InterpreterTest, testVisitDecrementtNode) {
    auto context = makeMockContext();
    context.getSymbolTable().set("x", std::make_unique<IntLiteral>(10));
    const std::unique_ptr<Node> mockNode = std::make_unique<VarDecrement>(Token(TokenType::IDENTIFIER, dummyPos, "x"));

    const std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* intLiteral = dynamic_cast<IntLiteral*>(result.get());
    ASSERT_NE(intLiteral, nullptr);
    EXPECT_EQ(intLiteral->getNumberValue(), 9);
}

TEST(InterpreterTest, testVisitIfStatementNode) {
    auto context = makeMockContext();
    std::unique_ptr<Node> comparison = makeNumbernode(10);

    std::vector<std::unique_ptr<Node>> ifNodes;
    ifNodes.push_back(std::make_unique<Number>(Token(TokenType::FLOAT, dummyPos, 5.5f)));
    std::vector<std::unique_ptr<Node>> elseNodes;
    elseNodes.push_back(std::make_unique<StringNode>(Token(TokenType::STRING, dummyPos, "testString")));

    const std::unique_ptr<Node> mockNode = std::make_unique<IfStmt>(std::move(comparison), std::move(ifNodes), std::move(elseNodes));
    std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* boolLiteral = dynamic_cast<IntLiteral*>(result.get());
    ASSERT_NE(boolLiteral, nullptr);
    EXPECT_EQ(boolLiteral->getNumberValue(), 10);
}

TEST(InterpreterTest, testVisitWhileStatementNode) {
    auto context = makeMockContext();
    context.getSymbolTable().set("x", std::make_unique<IntLiteral>(0));
    std::unique_ptr<Node> mockLeftNode = std::make_unique<VarAccess>(Token(TokenType::IDENTIFIER, dummyPos, "x"));
    Operator operatorNode = Operator(Token(TokenType::LESSTHAN, dummyPos));
    std::unique_ptr<Node> mockRightNode = makeNumbernode(10);
    std::unique_ptr<Node> comparison = std::make_unique<BinaryOperator>(std::move(mockLeftNode), operatorNode, std::move(mockRightNode));
    std::vector<std::unique_ptr<Node>> whileNodes;
    whileNodes.push_back(std::make_unique<VarIncrement>(Token(TokenType::IDENTIFIER, dummyPos, "x")));
    const std::unique_ptr<Node> mockNode = std::make_unique<WhileStmt>(std::move(comparison), std::move(whileNodes));
    std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* boolLiteral = dynamic_cast<BoolLiteral*>(result.get());
    ASSERT_NE(boolLiteral, nullptr);
    EXPECT_EQ(boolLiteral->getNumberValue(), true);
    EXPECT_EQ(context.getSymbolTable().getLiteral("x")->getNumberValue(), 10);
}

TEST(InterpreterTest, testVisitLibCall) {
    auto context = makeMockContext();
    std::vector<std::unique_ptr<Node>> args;
    args.push_back(makeNumbernode(5));
    const std::unique_ptr<Node> mockNode = std::make_unique<LibCall>(Token(TokenType::KEYWORD, dummyPos, "out"), std::move(args));
    std::stringstream buffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());
    Interpreter::visit(mockNode, &context);
    std::cout.rdbuf(oldCoutBuffer);
    std::string output = buffer.str();
    EXPECT_NE(output.find("5"), std::string::npos);
}

TEST(InterpreterTest, testVisitFuncDefinition) {
    auto context = makeMockContext();
    std::vector<Token> args;
    args.push_back(Token(TokenType::IDENTIFIER, dummyPos, "argName"));
    std::vector<std::unique_ptr<Node>> body;
    body.push_back(std::make_unique<ReturnCall>(Token(TokenType::KEYWORD, dummyPos, "return"), makeNumbernode(10)));
    const std::unique_ptr<Node> mockNode = std::make_unique<FuncDef>(
        Token(TokenType::IDENTIFIER, dummyPos, "testFunc"),
        std::move(args),
        std::move(body));
    std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* funcLiteral = dynamic_cast<FunctionLiteral*>(result.get());
    ASSERT_NE(funcLiteral, nullptr);
    auto* symbolFuncLiteral = dynamic_cast<FunctionLiteral*>(context.getSymbolTable().getLiteral("testFunc"));
    ASSERT_NE(symbolFuncLiteral, nullptr);
}

TEST(InterpreterTest, testVisitFuncCall) {
    auto context = makeMockContext();
    std::unique_ptr<Context> funcContext = std::make_unique<Context>(makeMockContext());
    std::vector<Token> args;
    args.push_back(Token(TokenType::IDENTIFIER, dummyPos, "argName"));
    std::vector<std::unique_ptr<Node>> body;
    body.push_back(std::make_unique<ReturnCall>(Token(TokenType::KEYWORD, dummyPos, "return"), makeNumbernode(10)));
    std::unique_ptr<Literal> mockFunc = std::make_unique<FunctionLiteral>("testFunc", std::move(args), std::move(body), std::move(funcContext));
    mockFunc->setContext(&context);
    mockFunc->setPosition(dummyPos);
    context.getSymbolTable().set("testFunc", std::move(mockFunc));
    std::vector<std::unique_ptr<Node>> callArgs;
    callArgs.push_back( makeNumbernode(10));
    const std::unique_ptr<Node> mockNode = std::make_unique<FuncCall>(
        Token(TokenType::IDENTIFIER, dummyPos, "testFunc"),
        std::move(callArgs)
    );
    std::unique_ptr<Literal> result = Interpreter::visit(mockNode, &context);
    ASSERT_NE(result, nullptr);
    auto* numberLiteral = dynamic_cast<NumberLiteral*>(result.get());
    ASSERT_NE(numberLiteral, nullptr);
    ASSERT_EQ(numberLiteral->getNumberValue(), 10);
}


