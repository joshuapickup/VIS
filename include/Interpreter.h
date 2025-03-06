//
// Created by joshu on 29/10/2024.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>

#include "Node.h"


class Interpreter {
public:
    explicit Interpreter();
    void visit(std::unique_ptr<Node> &node);
private:
    void visitNumberNode(Number* node);
    void visitBinaryOpNode(BinaryOperator* node);
    void visitUnaryOpNode(UnaryOperator* node);
    void visitVarAccessNode(VarAccess* node);
    void visitVarAssignNode(VarAssignment* node);
};



#endif //INTERPRETER_H
