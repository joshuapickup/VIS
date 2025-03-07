//
// Created by joshu on 29/10/2024.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>

#include "Node.h"
#include "Context.h"


class Interpreter {
public:
    explicit Interpreter();
    void visit(std::unique_ptr<Node> &node, Context* context);
private:
    void visitNumberNode(Number* node, Context* context);
    void visitBinaryOpNode(BinaryOperator* node, Context* context);
    void visitUnaryOpNode(UnaryOperator* node, Context* context);
    void visitVarAccessNode(VarAccess* node, Context* context);
    void visitVarAssignNode(VarAssignment* node, Context* context);
};



#endif //INTERPRETER_H
