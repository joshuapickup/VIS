//
// Created by joshu on 29/10/2024.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>

#include "Node.h"
#include "Context.h"

void printTokens(const std::map<int, std::vector<Token>>& tokenMap);

class ReturnSignal {
public:
    explicit ReturnSignal(std::unique_ptr<Literal> value);
    std::unique_ptr<Literal> getValue();
private:
    std::unique_ptr<Literal> returnValue;
};

class Interpreter {
public:
    explicit Interpreter(const std::string &filename, bool verboseFlag);
private:
    static void interpretFile(const std::string &filename, bool verboseFlag);
    static std::unique_ptr<Literal> visit(const std::unique_ptr<Node> &node, Context* context);
    static std::unique_ptr<Literal> visitNumberNode(const Number* node, Context* context);
    static std::unique_ptr<Literal> visitStringNode(const StringNode* node, Context* context);
    static std::unique_ptr<Literal> visitBinaryOpNode(const BinaryOperator* node, Context* context);
    static std::unique_ptr<Literal> visitUnaryOpNode(const UnaryOperator* node, Context* context);
    static std::unique_ptr<Literal> visitVarAccessNode(const VarAccess* node, Context* context);
    static std::unique_ptr<Literal> visitVarAssignNode(const VarAssignment* node, Context* context);
    static std::unique_ptr<Literal> visitVarIncrementNode(const VarIncrement* node, Context* context);
    static std::unique_ptr<Literal> visitVarDecrementNode(const VarDecrement* node, Context* context);
    static std::unique_ptr<Literal> visitLibCallNode(const LibCall* node, Context* context);
    static std::unique_ptr<Literal> visitIfStmtNode(const IfStmt* node, Context* context);
    static std::unique_ptr<Literal> visitWhileStmtNode(const WhileStmt* node, Context* context);
    static std::unique_ptr<Literal> visitForStmtNode(const ForStmt* node, Context* context);
    static std::unique_ptr<Literal> visitFuncDefNode(const FuncDef* node, Context* context);
    static std::unique_ptr<Literal> visitFuncCallNode(const FuncCall* node, Context* context);
    static std::unique_ptr<Literal> visitReturnCallNode(const ReturnCall* node, Context* context);
};



#endif //INTERPRETER_H
