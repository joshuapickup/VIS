#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "TestHelpers.h"
#include "Parser.h"
#include "Token.h"
#include "Node.h"

TEST(ParserTest, ParsesSimpleVariableAssignment) {
    std::vector<Token> tokens = {
        Token(TokenType::KEYWORD, dummyPos, "var"),
        Token(TokenType::IDENTIFIER, dummyPos, "x"),
        Token(TokenType::EQUALS, dummyPos),
        Token(TokenType::INT, dummyPos, 42),
        Token(TokenType::EOL, dummyPos)
    };
    std::map<int, std::vector<Token>> tokenMap = { {0, tokens}, {1, {Token(TokenType::EOF_, dummyPos)}} };
    Parser parser(tokenMap);
    std::unique_ptr<Node> node = parser.parse();
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), NodeType::VarAssgnment);
    auto* varAssign = dynamic_cast<VarAssignment*>(node.get());
    ASSERT_NE(varAssign, nullptr);
    EXPECT_EQ(std::get<std::string>(varAssign->getToken().getValue()), "x");
    auto* valueNode = dynamic_cast<Number*>(varAssign->getValue().get());
    ASSERT_NE(valueNode, nullptr);
    EXPECT_EQ(valueNode->getType(), NodeType::Number);
    EXPECT_EQ(std::get<int>(valueNode->getToken().getValue()), 42);
}

TEST(ParserTest, ParsesIncorrectSyntax) {
    std::vector<Token> tokens = {
        Token(TokenType::KEYWORD, dummyPos, "var"),
        Token(TokenType::KEYWORD, dummyPos, "var"),
        Token(TokenType::EQUALS, dummyPos),
        Token(TokenType::INT, dummyPos, 42),
        Token(TokenType::EOL, dummyPos)
    };
    std::map<int, std::vector<Token>> tokenMap = { {0, tokens}, {1, {Token(TokenType::EOF_, dummyPos)}} };
    Parser parser(tokenMap);
    EXPECT_THROW(parser.parse(), InvalidSyntaxError);
}

TEST(ParserTest, ParsesFuncCall) {
    std::vector<Token> tokens = {
        Token(TokenType::IDENTIFIER, dummyPos, "testName"),
        Token(TokenType::OPENPAREN, dummyPos),
        Token(TokenType::STRING, dummyPos, "test"),
        Token(TokenType::CLOSEPAREN, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::map<int, std::vector<Token>> tokenMap = {
        {0, tokens},
        {1, {Token(TokenType::EOF_, dummyPos)}}
    };
    Parser parser(tokenMap);
    std::unique_ptr<Node> result = parser.parse();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), NodeType::FuncCall);
    auto* call = dynamic_cast<FuncCall*>(result.get());
    ASSERT_NE(call, nullptr);
    const auto& arguments = call->getArguments();
    ASSERT_EQ(arguments.size(), 1);  // One expression inside the true block
    const Node* argument = arguments.at(0).get();
    EXPECT_EQ(argument->getType(), NodeType::String);
}

TEST(ParserTest, ParsesLibCallToOutWithString) {
    std::vector<Token> tokens = {
        Token(TokenType::KEYWORD, dummyPos, "out"),
        Token(TokenType::OPENPAREN, dummyPos),
        Token(TokenType::STRING, dummyPos, "test"),
        Token(TokenType::CLOSEPAREN, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::map<int, std::vector<Token>> tokenMap = {
        {0, tokens},
        {1, {Token(TokenType::EOF_, dummyPos)}}
    };
    Parser parser(tokenMap);
    std::unique_ptr<Node> result = parser.parse();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getType(), NodeType::LibCall);
    auto* call = dynamic_cast<LibCall*>(result.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(std::get<std::string>(call->getToken().getValue()), "out");
}

TEST(ParserTest, ParsesFuncDefinition) {
    std::vector<Token> tokenLine1 = {
        Token(TokenType::KEYWORD, dummyPos, "func"),
        Token(TokenType::IDENTIFIER, dummyPos, "testFunc"),
        Token(TokenType::OPENPAREN, dummyPos),
        Token(TokenType::CLOSEPAREN, dummyPos),
        Token(TokenType::OPENBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine2 = {
        Token(TokenType::KEYWORD, dummyPos, "return"),
        Token(TokenType::INT, dummyPos, 5),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine3 = {
        Token(TokenType::CLOSEBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    const std::map<int, std::vector<Token>> tokenMap =
        {
        {0, tokenLine1},
        {1, tokenLine2},
        {2, tokenLine3},
        {3, {Token(TokenType::EOF_, dummyPos)} }
        };

    Parser parser(tokenMap);
    // Act: Parse the tokens
    const std::unique_ptr<Node> node = parser.parse();

    // Assert: Check the returned Node
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), NodeType::FuncDef);

    auto* funcDefinition = dynamic_cast<FuncDef*>(node.get());
    ASSERT_NE(funcDefinition, nullptr);
    EXPECT_EQ(funcDefinition->getName(), "testFunc");
    EXPECT_EQ(funcDefinition->getArguments().size(), 0);

    const auto& bodyNodes = funcDefinition->getFunctionBody();
    ASSERT_EQ(bodyNodes.size(), 1);

    const Node* bodyNode = bodyNodes.at(0).get();
    ASSERT_NE(bodyNode, nullptr);
    EXPECT_EQ(bodyNode->getType(), NodeType::ReturnCall);

    const ReturnCall* returnCast = dynamic_cast<const ReturnCall*>(bodyNode);
    ASSERT_NE(returnCast, nullptr);

    const Node* valueNode = returnCast->getExpression().get();
    ASSERT_NE(valueNode, nullptr);
    EXPECT_EQ(valueNode->getType(), NodeType::Number);

    const auto* numberNode = dynamic_cast<const Number*>(valueNode);
    ASSERT_NE(numberNode, nullptr);
    EXPECT_EQ(std::get<int>(numberNode->getToken().getValue()), 5);
}

TEST(ParserTest, ParsesIfSatement) {
    std::vector<Token> tokenLine1 = {
        Token(TokenType::KEYWORD, dummyPos, "if"),
        Token(TokenType::OPENPAREN, dummyPos),
        Token(TokenType::INT, dummyPos, 3),
        Token(TokenType::CLOSEPAREN, dummyPos),
        Token(TokenType::OPENBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine2 = {
        Token(TokenType::INT, dummyPos, 1),
        Token(TokenType::PLUS, dummyPos),
        Token(TokenType::INT, dummyPos, 2),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine3 = {
        Token(TokenType::CLOSEBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine4 = {
        Token(TokenType::KEYWORD, dummyPos, "else"),
        Token(TokenType::OPENBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine5 = {
        Token(TokenType::INT, dummyPos, 3),
        Token(TokenType::MINUS, dummyPos),
        Token(TokenType::INT, dummyPos, 4),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine6 = {
        Token(TokenType::CLOSEBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    const std::map<int, std::vector<Token>> tokenMap =
        {
        {0, tokenLine1},
        {1, tokenLine2},
        {2, tokenLine3},
        {3, tokenLine4},
        {4, tokenLine5},
        {5, tokenLine6},
        {6, {Token(TokenType::EOF_, dummyPos)} }
        };

    Parser parser(tokenMap);
    const std::unique_ptr<Node> node = parser.parse();
    // Assert: Check the returned Node
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), NodeType::IfStmt);
    auto* ifStatement = dynamic_cast<IfStmt*>(node.get());
    ASSERT_NE(ifStatement, nullptr);
    // Check the condition node
    const Node* condition = ifStatement->getComparison().get();
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->getType(), NodeType::Number);
    const auto* conditionNumber = dynamic_cast<const Number*>(condition);
    ASSERT_NE(conditionNumber, nullptr);
    EXPECT_EQ(std::get<int>(conditionNumber->getToken().getValue()), 3);
    // Check the true branch (then block)
    const auto& trueBody = ifStatement->getIfBlock();
    ASSERT_EQ(trueBody.size(), 1);  // One expression inside the true block

    const Node* trueExpr = trueBody.at(0).get();
    ASSERT_NE(trueExpr, nullptr);
    EXPECT_EQ(trueExpr->getType(), NodeType::BinaryOperator);

    const auto* trueBinOp = dynamic_cast<const BinaryOperator*>(trueExpr);
    ASSERT_NE(trueBinOp, nullptr);
    const Node* leftOperand = trueBinOp->getLeftNode().get();
    const Node* rightOperand = trueBinOp->getRightNode().get();
    ASSERT_NE(leftOperand, nullptr);
    ASSERT_NE(rightOperand, nullptr);

    EXPECT_EQ(leftOperand->getType(), NodeType::Number);
    EXPECT_EQ(rightOperand->getType(), NodeType::Number);

    const auto* leftNum = dynamic_cast<const Number*>(leftOperand);
    const auto* rightNum = dynamic_cast<const Number*>(rightOperand);
    ASSERT_NE(leftNum, nullptr);
    ASSERT_NE(rightNum, nullptr);
    EXPECT_EQ(std::get<int>(leftNum->getToken().getValue()), 1);
    EXPECT_EQ(std::get<int>(rightNum->getToken().getValue()), 2);

    // Check that the binary operator is PLUS
    EXPECT_EQ(trueBinOp->getOperatorNode().getToken().getType(), TokenType::PLUS);

    // Check the false branch (else block)
    const auto& falseBody = ifStatement->getElseBlock();
    ASSERT_EQ(falseBody.size(), 1);  // One expression inside the else block

    const Node* falseExpr = falseBody.at(0).get();
    ASSERT_NE(falseExpr, nullptr);
    EXPECT_EQ(falseExpr->getType(), NodeType::BinaryOperator);

    const auto* falseBinOp = dynamic_cast<const BinaryOperator*>(falseExpr);
    ASSERT_NE(falseBinOp, nullptr);

    const Node* falseLeft = falseBinOp->getLeftNode().get();
    const Node* falseRight = falseBinOp->getRightNode().get();
    ASSERT_NE(falseLeft, nullptr);
    ASSERT_NE(falseRight, nullptr);

    EXPECT_EQ(falseLeft->getType(), NodeType::Number);
    EXPECT_EQ(falseRight->getType(), NodeType::Number);

    const auto* falseLeftNum = dynamic_cast<const Number*>(falseLeft);
    const auto* falseRightNum = dynamic_cast<const Number*>(falseRight);
    ASSERT_NE(falseLeftNum, nullptr);
    ASSERT_NE(falseRightNum, nullptr);
    EXPECT_EQ(std::get<int>(falseLeftNum->getToken().getValue()), 3);
    EXPECT_EQ(std::get<int>(falseRightNum->getToken().getValue()), 4);

    // Check that the binary operator is MINUS
    EXPECT_EQ(falseBinOp->getOperatorNode().getToken().getType(), TokenType::MINUS);

}

TEST(ParserTest, ParsesWhileSatement) {
    std::vector<Token> tokenLine1 = {
        Token(TokenType::KEYWORD, dummyPos, "while"),
        Token(TokenType::OPENPAREN, dummyPos),
        Token(TokenType::INT, dummyPos, 3),
        Token(TokenType::CLOSEPAREN, dummyPos),
        Token(TokenType::OPENBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine2 = {
        Token(TokenType::INT, dummyPos, 1),
        Token(TokenType::PLUS, dummyPos),
        Token(TokenType::INT, dummyPos, 2),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine3 = {
        Token(TokenType::CLOSEBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    const std::map<int, std::vector<Token>> tokenMap =
        {
        {0, tokenLine1},
        {1, tokenLine2},
        {2, tokenLine3},
        {6, {Token(TokenType::EOF_, dummyPos)} }
        };

    Parser parser(tokenMap);
    // Act: Parse the tokens
    const std::unique_ptr<Node> node = parser.parse();

    // Assert: Check the returned Node
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), NodeType::WhileStmt);

    auto* whileStatement = dynamic_cast<WhileStmt*>(node.get());
    ASSERT_NE(whileStatement, nullptr);

    // Check the condition node
    const Node* condition = whileStatement->getComparison().get();
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->getType(), NodeType::Number);

    const auto* conditionNumber = dynamic_cast<const Number*>(condition);
    ASSERT_NE(conditionNumber, nullptr);
    EXPECT_EQ(std::get<int>(conditionNumber->getToken().getValue()), 3);

    // Check the true branch (then block)
    const auto& loopBody = whileStatement->getWhileBlock();
    ASSERT_EQ(loopBody.size(), 1);  // One expression inside the true block

    const Node* trueExpr = loopBody.at(0).get();
    ASSERT_NE(trueExpr, nullptr);
    EXPECT_EQ(trueExpr->getType(), NodeType::BinaryOperator);

    const auto* trueBinOp = dynamic_cast<const BinaryOperator*>(trueExpr);
    ASSERT_NE(trueBinOp, nullptr);
    const Node* leftOperand = trueBinOp->getLeftNode().get();
    const Node* rightOperand = trueBinOp->getRightNode().get();
    ASSERT_NE(leftOperand, nullptr);
    ASSERT_NE(rightOperand, nullptr);

    EXPECT_EQ(leftOperand->getType(), NodeType::Number);
    EXPECT_EQ(rightOperand->getType(), NodeType::Number);

    const auto* leftNum = dynamic_cast<const Number*>(leftOperand);
    const auto* rightNum = dynamic_cast<const Number*>(rightOperand);
    ASSERT_NE(leftNum, nullptr);
    ASSERT_NE(rightNum, nullptr);

    EXPECT_EQ(std::get<int>(leftNum->getToken().getValue()), 1);
    EXPECT_EQ(std::get<int>(rightNum->getToken().getValue()), 2);

    // Check that the binary operator is PLUS
    EXPECT_EQ(trueBinOp->getOperatorNode().getToken().getType(), TokenType::PLUS);
}

TEST(ParserTest, ParsesForSatement) {
    std::vector<Token> tokenLine1 = {
        Token(TokenType::KEYWORD, dummyPos, "for"),
        Token(TokenType::OPENPAREN, dummyPos),
        Token(TokenType::KEYWORD, dummyPos, "var"),
        Token(TokenType::IDENTIFIER, dummyPos, "x"),
        Token(TokenType::EQUALS, dummyPos),
        Token(TokenType::INT, dummyPos, 0),
        Token(TokenType::SEPERATOR, dummyPos),
        Token(TokenType::IDENTIFIER, dummyPos, "x"),
        Token(TokenType::LESSTHAN, dummyPos),
        Token(TokenType::INT, dummyPos, 10),
        Token(TokenType::SEPERATOR, dummyPos),
        Token(TokenType::KEYWORD, dummyPos, "var"),
        Token(TokenType::IDENTIFIER, dummyPos, "x"),
        Token(TokenType::INCREMENT, dummyPos),
        Token(TokenType::CLOSEPAREN, dummyPos),
        Token(TokenType::OPENBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine2 = {
        Token(TokenType::INT, dummyPos, 1),
        Token(TokenType::PLUS, dummyPos),
        Token(TokenType::INT, dummyPos, 2),
        Token(TokenType::EOL, dummyPos)
    };
    std::vector<Token> tokenLine3 = {
        Token(TokenType::CLOSEBRACE, dummyPos),
        Token(TokenType::EOL, dummyPos)
    };
    const std::map<int, std::vector<Token>> tokenMap =
        {
        {0, tokenLine1},
        {1, tokenLine2},
        {2, tokenLine3},
        {6, {Token(TokenType::EOF_, dummyPos)} }
        };

    Parser parser(tokenMap);
    // Act: Parse the tokens
    const std::unique_ptr<Node> node = parser.parse();

    // Assert: Check the returned Node
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), NodeType::ForStmt);

    auto* whileStatement = dynamic_cast<ForStmt*>(node.get());
    ASSERT_NE(whileStatement, nullptr);

    // Check the condition node
    const Node* varDeclare = whileStatement->getVarDeclare().get();
    ASSERT_NE(varDeclare, nullptr);
    EXPECT_EQ(varDeclare->getType(), NodeType::VarAssgnment);
    const auto* varAssignNode = dynamic_cast<const VarAssignment*>(varDeclare);
    ASSERT_NE(varAssignNode, nullptr);
    EXPECT_EQ(std::get<std::string>(varAssignNode->getToken().getValue()), "x");
    EXPECT_EQ(varAssignNode->getValue()->getType(), NodeType::Number);

    // Check the true branch (then block)
    const auto& loopBody = whileStatement->getForBlock();
    ASSERT_EQ(loopBody.size(), 1);  // One expression inside the true block

    const Node* trueExpr = loopBody.at(0).get();
    ASSERT_NE(trueExpr, nullptr);
    EXPECT_EQ(trueExpr->getType(), NodeType::BinaryOperator);

    const auto* trueBinOp = dynamic_cast<const BinaryOperator*>(trueExpr);
    ASSERT_NE(trueBinOp, nullptr);
    const Node* leftOperand = trueBinOp->getLeftNode().get();
    const Node* rightOperand = trueBinOp->getRightNode().get();
    ASSERT_NE(leftOperand, nullptr);
    ASSERT_NE(rightOperand, nullptr);

    EXPECT_EQ(leftOperand->getType(), NodeType::Number);
    EXPECT_EQ(rightOperand->getType(), NodeType::Number);

    const auto* leftNum = dynamic_cast<const Number*>(leftOperand);
    const auto* rightNum = dynamic_cast<const Number*>(rightOperand);
    ASSERT_NE(leftNum, nullptr);
    ASSERT_NE(rightNum, nullptr);

    EXPECT_EQ(std::get<int>(leftNum->getToken().getValue()), 1);
    EXPECT_EQ(std::get<int>(rightNum->getToken().getValue()), 2);

    // Check that the binary operator is PLUS
    EXPECT_EQ(trueBinOp->getOperatorNode().getToken().getType(), TokenType::PLUS);
}



