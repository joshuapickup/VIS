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

std::vector<std::unique_ptr<Node>> Node::cloneNodeVector(const std::vector<std::unique_ptr<Node>>& nodes) {
    std::vector<std::unique_ptr<Node>> result;
    result.reserve(nodes.size());
    for (const auto& node : nodes) {
        result.push_back(node->clone());
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Node &node) {
    node.printNode(os, 0);
    return os;
}



//END OF FILE DEFINITION
EndOfFile::EndOfFile(const Token &token): Node(token, NodeType::EndOfFile) {}

std::unique_ptr<Node> EndOfFile::clone() const {return std::make_unique<EndOfFile>(*this);}

void EndOfFile::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "End Of File <>" << std::endl;
}



//NUMBER DEFINTITION
Number::Number(const Token &token): Node(token, NodeType::Number) {}

std::unique_ptr<Node> Number::clone() const {return std::make_unique<Number>(*this);}

void Number::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Number Node <" << tokenVector[0] << ">" << std::endl;
}



//OPERATOR DEFINITION
Operator::Operator(const Token &token) : Node(token, NodeType::Operator){}

std::unique_ptr<Node> Operator::clone() const {return std::make_unique<Operator>(*this);}

void Operator::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Operator Node <" << tokenVector[0] << ">" << std::endl;
}



//UNARY OPERATOR DEFINITION
UnaryOperator::UnaryOperator(const Operator &operator_, std::unique_ptr<Node> node) :
                            Node(operator_.getToken(), NodeType::UnaryOperator),
                            operatorNode(operator_), valueNode(std::move(node)) {
    tokenVector = {operatorNode.getToken(), this->valueNode->getToken()};
}

std::vector<Token> UnaryOperator::getTokens() const {return tokenVector;}

Operator UnaryOperator::getOperator() const {return operatorNode;}

const std::unique_ptr<Node>& UnaryOperator::getValue() const {return valueNode;}

std::unique_ptr<Node> UnaryOperator::clone() const {
    return std::make_unique<UnaryOperator>(operatorNode, valueNode->clone());
}

void UnaryOperator::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "UnaryOpNode<" << std::endl;
    operatorNode.printNode(os, tabCount+1);
    valueNode->printNode(os, tabCount+1);
    os << std::string(tabCount, '\t') << "UnaryOpNode>" << std::endl;
}



//BINARY OPERATOR DEFINITION
BinaryOperator::BinaryOperator(
    std::unique_ptr<Node> leftNode,
    const Operator &operatorNode,
    std::unique_ptr<Node> rightNode
    ):
        Node(leftNode->getToken(), NodeType::BinaryOperator),
        leftNode(std::move(leftNode)),
        operatorNode(operatorNode),
        rightNode(std::move(rightNode)) {
    tokenVector = {this->leftNode->getToken(), operatorNode.getToken(), this->rightNode->getToken()};
}

std::vector<Token> BinaryOperator::getTokens() {return tokenVector;}

const std::unique_ptr<Node>& BinaryOperator::getLeftNode() const {return leftNode;}

Operator BinaryOperator::getOperatorNode() const {return operatorNode;}

const std::unique_ptr<Node>& BinaryOperator::getRightNode() const {return rightNode;}

std::unique_ptr<Node> BinaryOperator::clone() const {
    return std::make_unique<BinaryOperator>(
        leftNode->clone(),
        operatorNode,
        rightNode->clone()
    );
}

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

std::unique_ptr<Node> VarAssignment::clone() const {
    return std::make_unique<VarAssignment>(getToken(), value->clone());
}

void VarAssignment::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "VarAssignNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << tokenVector[0] << std::endl;
    value->printNode(os, tabCount+1);
    os << std::string(tabCount, '\t') << "VarAssignNode>" << std::endl;
}



// VAR ACCESS DEFINITION
VarAccess::VarAccess(const Token &token): Node(token, NodeType::VarAccess) {}
std::unique_ptr<Node> VarAccess::clone() const {return std::make_unique<VarAccess>(*this);}
void VarAccess::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "VarAccessNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "VariableName: " + std::get<std::string>(getToken().getValue()) << std::endl;
    os << std::string(tabCount, '\t') << "VarAccessNode>" << std::endl;
}

// VAR INCREMENT DEFINITION
VarIncrement::VarIncrement(const Token &token): Node(token, NodeType::VarIncrement) {}
std::unique_ptr<Node> VarIncrement::clone() const {return std::make_unique<VarIncrement>(*this);}
void VarIncrement::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "VarIncrementNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "VariableName: " + std::get<std::string>(getToken().getValue()) << std::endl;
    os << std::string(tabCount, '\t') << "VarIncrementNode>" << std::endl;
}

// VAR DECREMENT DEFINITION
VarDecrement::VarDecrement(const Token &token): Node(token, NodeType::VarDecrement) {}
std::unique_ptr<Node> VarDecrement::clone() const {return std::make_unique<VarDecrement>(*this);}
void VarDecrement::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "VarDecrementNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "VariableName: " + std::get<std::string>(getToken().getValue()) << std::endl;
    os << std::string(tabCount, '\t') << "VarDecrementNode>" << std::endl;
}



// IF STATEMENT DEFINITION
IfStmt::IfStmt(
    std::unique_ptr<Node> comparisonNode,
    std::vector<std::unique_ptr<Node>> ifBlockNodes,
    std::vector<std::unique_ptr<Node>> elseBlockNodes
    ) :
Node(comparisonNode->getToken(), NodeType::IfStmt),
comparison(std::move(comparisonNode)),
ifBlockNodes(std::move(ifBlockNodes)),
elseBlockNodes(std::move(elseBlockNodes)) {}

const std::unique_ptr<Node> & IfStmt::getComparison() const {return comparison;}

const std::vector<std::unique_ptr<Node>>& IfStmt::getIfBlock() const {return ifBlockNodes;}

const std::vector<std::unique_ptr<Node>>& IfStmt::getElseBlock() const {return elseBlockNodes;}

std::unique_ptr<Node> IfStmt::clone() const {
    auto clonedIf = std::vector<std::unique_ptr<Node>>{};
    for (const auto& stmt : ifBlockNodes)
        clonedIf.push_back(stmt->clone());

    auto clonedElse = std::vector<std::unique_ptr<Node>>{};
    for (const auto& stmt : elseBlockNodes)
        clonedElse.push_back(stmt->clone());

    return std::make_unique<IfStmt>(
        comparison->clone(),
        std::move(clonedIf),
        std::move(clonedElse)
    );
}

void IfStmt::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "IfStatementNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Requirement<" << std::endl;
    comparison->printNode(os, tabCount+2);
    os << std::string(tabCount+1, '\t') << "Requirement>" << std::endl;
    os << std::string(tabCount+1, '\t') << "IfStatements<" << std::endl;
    for (const auto& node : ifBlockNodes) {node->printNode(os, tabCount+2);}
    os << std::string(tabCount+1, '\t') << "IfStatements>" << std::endl;
    os << std::string(tabCount+1, '\t') << "ElseStatements<" << std::endl;
    for (const auto& node : elseBlockNodes) {node->printNode(os, tabCount+2);}
    os << std::string(tabCount+1, '\t') << "ElseStatements<" << std::endl;
    os << std::string(tabCount, '\t') << "IfStatementNode>" << std::endl;
}



// WHILE STATEMENT DEFINITION
WhileStmt::WhileStmt(
    std::unique_ptr<Node> comparisonNode,
    std::vector<std::unique_ptr<Node>> whileNodes
    ) :
Node(comparisonNode->getToken(), NodeType::WhileStmt),
comparison(std::move(comparisonNode)),
whileNodes(std::move(whileNodes)) {}

const std::unique_ptr<Node> & WhileStmt::getComparison() const {return comparison;}

const std::vector<std::unique_ptr<Node>>& WhileStmt::getWhileBlock() const {return whileNodes;}

std::unique_ptr<Node> WhileStmt::clone() const {
    std::vector<std::unique_ptr<Node>> clonedBody = cloneNodeVector(whileNodes);
    return std::make_unique<WhileStmt>(
        comparison->clone(),
        std::move(clonedBody)
    );
}

void WhileStmt::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "WhileStatementNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Condition<" << std::endl;
    comparison->printNode(os, tabCount+2);
    os << std::string(tabCount+1, '\t') << "Condition>" << std::endl;
    os << std::string(tabCount+1, '\t') << "WhileStatements<" << std::endl;
    for (const auto& node : whileNodes) {node->printNode(os, tabCount+2);}
    os << std::string(tabCount+1, '\t') << "WhileStatements>" << std::endl;
    os << std::string(tabCount, '\t') << "WhileStatementNode>" << std::endl;
}



// FOR STATEMENT DEFINITION
ForStmt::ForStmt(
    std::unique_ptr<Node> varInitialisation,
    std::unique_ptr<Node> condition,
    std::unique_ptr<Node> step,
    std::vector<std::unique_ptr<Node>> forNodes
    ) :
Node(varInitialisation->getToken(), NodeType::ForStmt),
varDeclare(std::move(varInitialisation)),
condition(std::move(condition)),
step(std::move(step)),
forNodes(std::move(forNodes)) {}

const std::unique_ptr<Node> & ForStmt::getVarDeclare() const {return varDeclare;}

const std::unique_ptr<Node> & ForStmt::getCondition() const {return condition;}

const std::unique_ptr<Node> & ForStmt::getStep() const {return step;}

const std::vector<std::unique_ptr<Node>>& ForStmt::getForBlock() const {return forNodes;}

std::unique_ptr<Node> ForStmt::clone() const {
    std::vector<std::unique_ptr<Node>> clonedBody = cloneNodeVector(forNodes);
    return std::make_unique<ForStmt>(
        varDeclare->clone(),
        condition->clone(),
        step->clone(),
        std::move(clonedBody)
    );
}

void ForStmt::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "ForStatementNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Decleration<" << std::endl;
    varDeclare->printNode(os, tabCount+2);
    os << std::string(tabCount+1, '\t') << "Decleration>" << std::endl;
    os << std::string(tabCount+1, '\t') << "Condition<" << std::endl;
    condition->printNode(os, tabCount+2);
    os << std::string(tabCount+1, '\t') << "Condition>" << std::endl;
    os << std::string(tabCount+1, '\t') << "Step<" << std::endl;
    step->printNode(os, tabCount+2);
    os << std::string(tabCount+1, '\t') << "Step>" << std::endl;
    os << std::string(tabCount+1, '\t') << "ForStatements<" << std::endl;
    for (const auto& node : forNodes) {node->printNode(os, tabCount+2);}
    os << std::string(tabCount+1, '\t') << "ForStatements>" << std::endl;
    os << std::string(tabCount, '\t') << "ForStatementNode>" << std::endl;
}



// FUNCTION DEF DEFINITION
FuncDef::FuncDef(
    const Token &token,
    std::vector<Token> arguments,
    std::vector<std::unique_ptr<Node>> bodyNodes
    ) :
Node(token, NodeType::FuncDef),
arguments(std::move(arguments)),
bodyNodes(std::move(bodyNodes)) {}

std::string FuncDef::getName() const {return std::get<std::string>(getToken().getValue());}

const std::vector<Token>& FuncDef::getArguments() const {return arguments;}

const std::vector<std::unique_ptr<Node>> & FuncDef::getFunctionBody() const {return bodyNodes;}

std::unique_ptr<Node> FuncDef::clone() const {
    std::vector<Token> clonedArgs = {};
    clonedArgs.reserve(this->arguments.size());
    for (const Token& token : this->arguments) {clonedArgs.push_back(token.clone());}
    std::vector<std::unique_ptr<Node>> clonedBody = cloneNodeVector(bodyNodes);
    return std::make_unique<FuncDef>(getToken(), std::move(clonedArgs), std::move(clonedBody));
}

void FuncDef::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "FunctionDeclerationNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Name: " << std::get<std::string>(getToken().getValue()) << std::endl;
    os << std::string(tabCount+1, '\t') << "Arguments<" << std::endl;
    for (const auto& token : arguments) {
        os << std::string(tabCount+1, '\t') << "Name: " << std::get<std::string>(token.getValue()) << std::endl;
    }
    os << std::string(tabCount+1, '\t') << "Arguments>" << std::endl;
    os << std::string(tabCount+1, '\t') << "StatementNodes<" << std::endl;
    for (const auto& node : bodyNodes) {node->printNode(os, tabCount+2);}
    os << std::string(tabCount+1, '\t') << "StatementNodes>" << std::endl;
    os << std::string(tabCount, '\t') << "FunctionDeclerationNode>" << std::endl;
}



// FUNCTION CALL DEFINITION
FuncCall::FuncCall(
    const Token &token,
    std::vector<std::unique_ptr<Node>> argumentNodes
    ) :
Node(token, NodeType::FuncCall),
name(std::get<std::string>(getToken().getValue())),
argumentNodes(std::move(argumentNodes)) {}

std::string FuncCall::getName() const {return name;}

const std::vector<std::unique_ptr<Node>> & FuncCall::getArguments() const {return argumentNodes;}

std::unique_ptr<Node> FuncCall::clone() const {
    std::vector<std::unique_ptr<Node>> clonedArgs = cloneNodeVector(argumentNodes);

    return std::make_unique<FuncCall>(getToken(), std::move(clonedArgs));
}

void FuncCall::printNode(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "FunctionCallNode<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Name: " << name << std::endl;
    os << std::string(tabCount+1, '\t') << "Arguments<" << std::endl;
    for (const auto& node : argumentNodes) {node->printNode(os, tabCount+2);}
    os << std::string(tabCount+1, '\t') << "Arguments>" << std::endl;
    os << std::string(tabCount, '\t') << "FunctionCallNode>" << std::endl;
}
