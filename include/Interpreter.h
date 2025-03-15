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
    std::unique_ptr<Literal> visit(const std::unique_ptr<Node> &node, Context* context);
private:
    static std::unique_ptr<Literal> visitNumberNode(const Number* node, Context* context);
    std::unique_ptr<Literal> visitBinaryOpNode(const BinaryOperator* node, Context* context);
    std::unique_ptr<Literal> visitUnaryOpNode(const UnaryOperator* node, Context* context);
    static std::unique_ptr<Literal> visitVarAccessNode(const VarAccess* node, Context* context);
    std::unique_ptr<Literal> visitVarAssignNode(const VarAssignment* node, Context* context);
};



#endif //INTERPRETER_H
