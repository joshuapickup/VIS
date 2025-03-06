//
// Created by joshu on 29/10/2024.
//

#include "Interpreter.h"

#include <iostream>


//INTERPRETER DEFINTITION
Interpreter::Interpreter() {

}

void Interpreter::visit(std::unique_ptr<Node> &node) {
    switch (node->getType()) {
        case NodeType::Number:
            visitNumberNode(dynamic_cast<Number*>(node.get()));
            break;
        case NodeType::BinaryOperator:
            visitBinaryOpNode(dynamic_cast<BinaryOperator*>(node.get()));
            break;
        case NodeType::UnaryOperator:
            visitUnaryOpNode(dynamic_cast<UnaryOperator*>(node.get()));
            break;
        case NodeType::VarAccess:
            visitVarAccessNode(dynamic_cast<VarAccess*>(node.get()));
        break;
        case NodeType::VarAssgnment:
            visitVarAssignNode(dynamic_cast<VarAssignment*>(node.get()));
        break;
        default:
            std::cout << "didnt recognise node" << std::endl;
            break;
    }
}

void Interpreter::visitNumberNode(Number* node) {
    std::cout << "in number node" << std::endl;
    std::cout << node->getToken() << std::endl;
}

void Interpreter::visitBinaryOpNode(BinaryOperator* node) {
    std::cout << "in binary op node" << std::endl;
    std::unique_ptr<Node> &left = node->getLeftNode();
    std::unique_ptr<Node> &right = node->getRightNode();
    visit(left);
    visit(right);
}

void Interpreter::visitUnaryOpNode(UnaryOperator* node) {
    std::cout << "in unary op node" << std::endl;
}

void Interpreter::visitVarAccessNode(VarAccess *node) {
    std::cout << "in var access node" << std::endl;
}

void Interpreter::visitVarAssignNode(VarAssignment *node) {
    std::cout << "in var assign node" << std::endl;
}


