#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>
#include "Token.h"

enum class NodeType {
    EndOfFile,
    Number,
    String,
    Operator,
    UnaryOperator,
    BinaryOperator,
    VarAssgnment,
    VarAccess,
    VarIncrement,
    VarDecrement,
    LibCall,
    IfStmt,
    WhileStmt,
    ForStmt,
    FuncDef,
    FuncCall,
    ReturnCall,
};

class Node {
public:
    virtual ~Node() = default;
    explicit Node(const Token &token, NodeType type_);
    [[nodiscard]] Token getToken() const;
    [[nodiscard]] NodeType getType() const;
    [[nodiscard]] virtual std::unique_ptr<Node> clone() const = 0;
    virtual void printNode(std::ostream& os, int tabCount) const;

    static std::vector<std::unique_ptr<Node>> cloneNodeVector(const std::vector<std::unique_ptr<Node>>& nodes);
    friend std::ostream& operator<<(std::ostream& os, const Node &node);
protected:
    std::vector<Token> tokenVector;
    NodeType type;
};

class EndOfFile final : public Node{
public:
    explicit EndOfFile(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class Number final : public Node{
public:
    explicit Number(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class StringNode final : public Node{
public:
    explicit StringNode(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class Operator final : public Node{
public:
    explicit Operator(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class UnaryOperator final : public Node{
public:
    UnaryOperator(const Operator &operator_, std::unique_ptr<Node> node);
    [[nodiscard]] std::vector<Token> getTokens() const;
    [[nodiscard]] Operator getOperator() const;
    [[nodiscard]] const std::unique_ptr<Node>& getValue() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    Operator operatorNode;
    std::unique_ptr<Node> valueNode;
};

class BinaryOperator final : public Node{
public:
    BinaryOperator(std::unique_ptr<Node> leftNode, const Operator &operatorNode, std::unique_ptr<Node> rightNode);
    std::vector<Token> getTokens();
    [[nodiscard]] const std::unique_ptr<Node>& getLeftNode() const;
    [[nodiscard]] Operator getOperatorNode() const;
    [[nodiscard]] const std::unique_ptr<Node>& getRightNode() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> leftNode;
    Operator operatorNode;
    std::unique_ptr<Node> rightNode;
};

class VarAssignment final : public Node{
public:
    VarAssignment(const Token &token, std::unique_ptr<Node> valueNode);
    [[nodiscard]] const std::unique_ptr<Node>& getValue() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> value;
};

class VarAccess final : public Node{
public:
    explicit VarAccess(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class VarIncrement final : public Node{
public:
    explicit VarIncrement(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class VarDecrement final : public Node{
public:
    explicit VarDecrement(const Token &token);
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
};

class LibCall final : public Node {
public:
    explicit LibCall(const Token& token, std::vector<std::unique_ptr<Node>> argument);
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getArgumentNodes() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::vector<std::unique_ptr<Node>> argumentNodes;
};

class IfStmt final : public Node{
public:
    explicit IfStmt(
        std::unique_ptr<Node> comparisonNode,
        std::vector<std::unique_ptr<Node>> ifBlockNodes,
        std::vector<std::unique_ptr<Node>> elseBlockNodes
        );
    [[nodiscard]] const std::unique_ptr<Node>& getComparison() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getIfBlock() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getElseBlock() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> comparison;
    std::vector<std::unique_ptr<Node>> ifBlockNodes;
    std::vector<std::unique_ptr<Node>> elseBlockNodes;
};

class WhileStmt final : public Node{
public:
    explicit WhileStmt(
        std::unique_ptr<Node> comparisonNode,
        std::vector<std::unique_ptr<Node>> whileNodes
        );
    [[nodiscard]] const std::unique_ptr<Node>& getComparison() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getWhileBlock() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> comparison;
    std::vector<std::unique_ptr<Node>> whileNodes;
};

class ForStmt final : public Node{
public:
    explicit ForStmt(
        std::unique_ptr<Node> varInitialisation,
        std::unique_ptr<Node> condition,
        std::unique_ptr<Node> step,
        std::vector<std::unique_ptr<Node>> forNodes
        );
    [[nodiscard]] const std::unique_ptr<Node>& getVarDeclare() const;
    [[nodiscard]] const std::unique_ptr<Node>& getCondition() const;
    [[nodiscard]] const std::unique_ptr<Node>& getStep() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getForBlock() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> varDeclare;
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> step;
    std::vector<std::unique_ptr<Node>> forNodes;
};

class FuncDef final : public Node{
public:
    explicit FuncDef(
        const Token &token,
        std::vector<Token> arguments,
        std::vector<std::unique_ptr<Node>> bodyNodes
        );
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const std::vector<Token>& getArguments() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getFunctionBody() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::vector<Token> arguments;
    std::vector<std::unique_ptr<Node>> bodyNodes;
};

class FuncCall final : public Node{
public:
    explicit FuncCall(const Token &token, std::vector<std::unique_ptr<Node>> argumentNodes);
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getArguments() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::string name;
    std::vector<std::unique_ptr<Node>> argumentNodes;
};

class ReturnCall final : public Node {
public:
    ReturnCall(const Token &token, std::unique_ptr<Node> expressionNode);
    [[nodiscard]] const std::unique_ptr<Node>& getExpression() const;
    [[nodiscard]] std::unique_ptr<Node> clone() const override;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> expression;
};

#endif //NODE_H
