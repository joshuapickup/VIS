#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>
#include "Token.h"

enum class NodeType {
    BaseNode,
    Number,
    Operator,
    UnaryOperator,
    BinaryOperator,
    VarAssgnment,
    VarAccess
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


#endif //NODE_H
