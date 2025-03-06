#include <iostream>
#include <utility>
#include "Node.h"

//NODE DEFINTITION
Node::Node(const Token &token, const NodeType type_) : tokenVector(std::vector<Token>{token}), type(type_){}

Token Node::getToken() const {return tokenVector[0];}

NodeType Node::getType() const {return type;}

void Node::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Node<" << std::endl;
    os << std::string(tabCount+1, '\t') << tokenVector[0] << std::endl;
    os << std::string(tabCount, '\t') << "Node>" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Node &node) {
    node.printNode(os, 0);
    return os;
}


//NUMBER DEFINTITION
Number::Number(const Token &token): Node(token, NodeType::Number) {}

void Number::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Number Node <" << tokenVector[0] << ">" << std::endl;
}

//OPERATOR DEFINITION
Operator::Operator(const Token &token) : Node(token, NodeType::Operator){}

void Operator::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Operator Node <" << tokenVector[0] << ">" << std::endl;
}


//UNARY OPERATOR DEFINITION
UnaryOperator::UnaryOperator(const Operator &operator_, std::unique_ptr<Node> node) :
                            Node(operator_.getToken(), NodeType::UnaryOperator),
                            operatorNode(operator_), valueNode(std::move(node)) {
    tokenVector = {operatorNode.getToken(), this->valueNode->getToken()};
}

std::vector<Token> UnaryOperator::getTokens() {return tokenVector;}

Node* UnaryOperator::getNode() const {return valueNode.get();}

void UnaryOperator::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "UnaryOpNode<" << std::endl;
    operatorNode.printNode(os, tabCount+1);
    valueNode->printNode(os, tabCount+1);
    os << std::string(tabCount, '\t') << "UnaryOpNode>" << std::endl;
}


//BINARY OPERATOR DEFINITION
BinaryOperator::BinaryOperator(std::unique_ptr<Node> leftNode, const Operator &operatorNode, std::unique_ptr<Node> rightNode):
                            Node(leftNode->getToken(), NodeType::BinaryOperator), leftNode(std::move(leftNode)),
                            operatorNode(operatorNode), rightNode(std::move(rightNode)) {
    tokenVector = {this->leftNode->getToken(), operatorNode.getToken(), this->rightNode->getToken()};
}

std::unique_ptr<Node>& BinaryOperator::getLeftNode() {return leftNode;}

Operator BinaryOperator::getOperatorNode() {return operatorNode;}

std::unique_ptr<Node>& BinaryOperator::getRightNode() {return rightNode;}

void BinaryOperator::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "BinOpNode<" << std::endl;
    leftNode->printNode(os, tabCount+1);
    operatorNode.printNode(os, tabCount+1);
    rightNode->printNode(os, tabCount+1);
    os << std::string(tabCount, '\t') << "BinOpNode>" << std::endl;
}


// VAR ASSIGNMENT DEFINITION
VarAssignment::VarAssignment(const Token &token, std::unique_ptr<Node> value_): Node(token, NodeType::VarAssgnment), value(std::move(value_)) {}

const std::unique_ptr<Node>& VarAssignment::getValue() const {return value;}

void VarAssignment::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "VarAssignment<" << std::endl;
    os << std::string(tabCount+1, '\t') << tokenVector[0] << std::endl;
    value->printNode(os, tabCount+1);
    os << std::string(tabCount, '\t') << "VarAssignment>" << std::endl;
}


// VAR ACCESS DEFINITION
VarAccess::VarAccess(const Token &token): Node(token, NodeType::VarAccess) {}

void VarAccess::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "VarAccessNode<" << std::endl;
    os << std::string(tabCount, '\t') << "VarAccessNode>" << std::endl;
}


