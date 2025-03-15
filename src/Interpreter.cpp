//
// Created by joshu on 29/10/2024.
//

#include "Interpreter.h"

#include <error.h>
#include <iostream>


//INTERPRETER DEFINTITION
Interpreter::Interpreter() = default;

std::unique_ptr<Literal> Interpreter::visit(const std::unique_ptr<Node> &node, Context *context) {
    switch (node->getType()) {
        case NodeType::Number:
            return visitNumberNode(dynamic_cast<Number*>(node.get()), context);
        case NodeType::BinaryOperator:
            return visitBinaryOpNode(dynamic_cast<BinaryOperator*>(node.get()), context);
        case NodeType::UnaryOperator:
            return visitUnaryOpNode(dynamic_cast<UnaryOperator*>(node.get()), context);
        case NodeType::VarAccess:
            return visitVarAccessNode(dynamic_cast<VarAccess*>(node.get()), context);
        break;
        case NodeType::VarAssgnment:
            return visitVarAssignNode(dynamic_cast<VarAssignment*>(node.get()), context);
        default:
            throw VisRunTimeError("visit node method not defined");
    }
    return {};
}

std::unique_ptr<Literal> Interpreter::visitNumberNode(const Number* node, Context* context) {
    const Token token = node->getToken();
    const TokenType type = token.getType();
    std::unique_ptr<Literal> numberLiteral = nullptr;
    if (type == TokenType::INT) {
        const int value = std::get<int>(token.getValue());
        numberLiteral = std::make_unique<IntLiteral>(value);
    }
    else if (type == TokenType::FLOAT) {
        const float value = std::get<float>(token.getValue());
        numberLiteral = std::make_unique<FloatLiteral>(value);
    }
    else {
        throw VisRunTimeError("When visiting number node was provided token of type <" + tokenTypeToStr(type) +
            "> instead of INT or FLOAT");
    }
    numberLiteral->setPosition(token.getPos());
    numberLiteral->setContext(context);
    return numberLiteral;
}

std::unique_ptr<Literal> Interpreter::visitBinaryOpNode(const BinaryOperator *node, Context *context) {
    const Token token = node->getToken();
    const std::unique_ptr<Node> &left = node->getLeftNode();
    const Operator operatorNode = node->getOperatorNode();
    const std::unique_ptr<Node> &right = node->getRightNode();
    const std::unique_ptr<Literal> leftvalue = visit(left, context);
    const std::unique_ptr<Literal> rightvalue = visit(right, context);
    std::unique_ptr<Literal> uniqueLiteral;
    switch (operatorNode.getToken().getType()) {
        case TokenType::PLUS:
            uniqueLiteral = leftvalue->add(*rightvalue);
            break;
        case TokenType::MINUS:
            uniqueLiteral = leftvalue->subtract(*rightvalue);
            break;
        case TokenType::MUL:
            uniqueLiteral = leftvalue->multiply(*rightvalue);
            break;
        case TokenType::DIV:
            uniqueLiteral = leftvalue->divide(*rightvalue);
            break;
        default:
            throw ParseError("did not recognise token <"
                + tokenTypeToStr(operatorNode.getToken().getType())
                + "> inside binary opertaion instead expected: PLUS, MINUS, MUL, DIV");
    }
    uniqueLiteral->setPosition(token.getPos());
    uniqueLiteral->setContext(context);

    return uniqueLiteral;
}

std::unique_ptr<Literal> Interpreter::visitUnaryOpNode(const UnaryOperator* node, Context* context) {
    const Token token = node->getToken();
    const Operator operatorNode = node->getOperator();
    const std::unique_ptr<Literal> valueLiteral = visit(node->getValue(), context);
    std::unique_ptr<Literal> returnLiteral;
    switch (operatorNode.getToken().getType()) {
        case TokenType::MINUS:
            returnLiteral = valueLiteral->multiply(IntLiteral(-1));
            break;
        default:
            throw ParseError("unknown operator <"
                + tokenTypeToStr(operatorNode.getToken().getType())
                + "> for unary operation, expected MINUS");
    }
    returnLiteral->setPosition(token.getPos());
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> Interpreter::visitVarAssignNode(const VarAssignment *node, Context* context) {
    const std::string varName = std::get<std::string>(node->getToken().getValue());
    auto literalValue = std::unique_ptr(visit(node->getValue(), context));
    std::unique_ptr clonedValue = literalValue->clone();
    context->getSymbolTable().set(varName, std::move(literalValue));
    return clonedValue;
}

std::unique_ptr<Literal> Interpreter::visitVarAccessNode(const VarAccess *node, Context* context) {
    std::cout << "in var access node" << std::endl;
    const std::string varName = std::get<std::string>(node->getToken().getValue());
    std::unique_ptr<Literal> value = context->getSymbolTable().getLiteral(varName)->clone();
    if (not value) {
        throw VisRunTimeError("unknown variable " + varName);
    }
    return value;
}


