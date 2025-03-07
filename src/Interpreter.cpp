//
// Created by joshu on 29/10/2024.
//

#include "Interpreter.h"

#include <error.h>
#include <iostream>


//INTERPRETER DEFINTITION
Interpreter::Interpreter() {

}

void Interpreter::visit(std::unique_ptr<Node> &node, Context* context) {
    switch (node->getType()) {
        case NodeType::Number:
            visitNumberNode(dynamic_cast<Number*>(node.get()), context);
            break;
        case NodeType::BinaryOperator:
            visitBinaryOpNode(dynamic_cast<BinaryOperator*>(node.get()), context);
            break;
        case NodeType::UnaryOperator:
            visitUnaryOpNode(dynamic_cast<UnaryOperator*>(node.get()), context);
            break;
        case NodeType::VarAccess:
            visitVarAccessNode(dynamic_cast<VarAccess*>(node.get()), context);
        break;
        case NodeType::VarAssgnment:
            visitVarAssignNode(dynamic_cast<VarAssignment*>(node.get()), context);
        break;
        default:
            throw VisRunTimeError("visit node method not defined");
    }
}

void Interpreter::visitNumberNode(Number* node, Context* context) {
    const Token token = node->getToken();



    std::cout << "in number node" << std::endl;
    std::cout << token << std::endl;
}

void Interpreter::visitBinaryOpNode(BinaryOperator* node, Context* context) {
    std::cout << "in binary op node" << std::endl;
    std::unique_ptr<Node> &left = node->getLeftNode();
    std::unique_ptr<Node> &right = node->getRightNode();
    visit(left, context);
    visit(right, context);
}

void Interpreter::visitUnaryOpNode(UnaryOperator* node, Context* context) {
    std::cout << "in unary op node" << std::endl;
}

void Interpreter::visitVarAccessNode(VarAccess *node, Context* context) {
    std::cout << "in var access node" << std::endl;
}

void Interpreter::visitVarAssignNode(VarAssignment *node, Context* context) {
    std::cout << "in var assign node" << std::endl;
}


