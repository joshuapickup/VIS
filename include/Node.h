#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>
#include "Token.h"

enum class NodeType {
    EndOfFile,
    Number,
    Operator,
    UnaryOperator,
    BinaryOperator,
    VarAssgnment,
    VarAccess,
    VarIncrement,
    VarDecrement,
    IfStmt,
    WhileStmt,
    ForStmt,
    FuncDef,
    FuncCall,
};

class Node {
public:
    virtual ~Node() = default;

    explicit Node(const Token &token, NodeType type_);
    [[nodiscard]] Token getToken() const;
    [[nodiscard]] NodeType getType() const;
    friend std::ostream& operator<<(std::ostream& os, const Node &node);
    virtual void printNode(std::ostream& os, int tabCount) const;
protected:
    std::vector<Token> tokenVector;
    NodeType type;
};

class EndOfFile final : public Node{
public:
    explicit EndOfFile(const Token &token);
    void printNode(std::ostream &os, int tabCount) const override;
};

class Number final : public Node{
public:
    explicit Number(const Token &token);
    void printNode(std::ostream &os, int tabCount) const override;
};

class Operator final : public Node{
public:
    explicit Operator(const Token &token);
    void printNode(std::ostream &os, int tabCount) const override;
};

class UnaryOperator final : public Node{
public:
    UnaryOperator(const Operator &operator_, std::unique_ptr<Node> node);
    [[nodiscard]] std::vector<Token> getTokens() const;
    [[nodiscard]] Operator getOperator() const;
    [[nodiscard]] const std::unique_ptr<Node>& getValue() const;
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
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::unique_ptr<Node> value;
};

class VarAccess final : public Node{
public:
    explicit VarAccess(const Token &token);
    void printNode(std::ostream &os, int tabCount) const override;
};

class VarIncrement final : public Node{
public:
    explicit VarIncrement(const Token &token);
    void printNode(std::ostream &os, int tabCount) const override;
};

class VarDecrement final : public Node{
public:
    explicit VarDecrement(const Token &token);
    void printNode(std::ostream &os, int tabCount) const override;
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
        std::vector<std::unique_ptr<Node>> arguments,
        std::vector<std::unique_ptr<Node>> bodyNodes
        );
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getArguments() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getFunctionBody() const;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::vector<std::unique_ptr<Node>> arguments;
    std::vector<std::unique_ptr<Node>> bodyNodes;
};

class FuncCall final : public Node{
public:
    explicit FuncCall(const Token &token, std::vector<std::unique_ptr<Node>> argumentNodes);
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getArguments() const;
    void printNode(std::ostream &os, int tabCount) const override;
private:
    std::string name;
    std::vector<std::unique_ptr<Node>> argumentNodes;
};

#endif //NODE_H
