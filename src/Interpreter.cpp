//
// Created by joshu on 29/10/2024.
//

#include <error.h>
#include <fstream>
#include <iostream>

#include "Interpreter.h"
#include "PositionHandler.h"
#include "Lexer.h"
#include "Parser.h"
#include "Literal.h"


void printTokens(const std::map<int, std::vector<Token>>& tokenMap) {
    for (const auto& [lineNumber, tokens] : tokenMap) {
        std::cout << "Line " << lineNumber << ":" << std::endl;
        for (const Token& token : tokens) {
            std::cout << token << std::endl;
        }
        std::cout << std::endl;
    }
}


//RETURN SIGNAL DEFINITION
ReturnSignal::ReturnSignal(std::unique_ptr<Literal> value) : returnValue(std::move(value)) {}
std::unique_ptr<Literal> ReturnSignal::getValue() {return std::move(returnValue);}



//INTERPRETER DEFINTITION
Interpreter::Interpreter(const std::string &filename, const bool verboseFlag) {
    interpretFile(filename, verboseFlag);
};

void Interpreter::interpretFile(const std::string &filename, bool verboseFlag) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {throw std::runtime_error("Error: Could not open file: " + filename);}
    PositionHandler positionHandler(filename, inputFile);
    SymbolTable globalSymbolTable = SymbolTable();
    globalSymbolTable.set("null", std::make_unique<BoolLiteral>(false));
    globalSymbolTable.set("true", std::make_unique<BoolLiteral>(true));
    globalSymbolTable.set("false", std::make_unique<BoolLiteral>(false));
    Context globalContext = Context(filename);
    globalContext.setSymbolTable(std::move(globalSymbolTable));

    Lexer lexer(positionHandler);
    std::map<int, std::vector<Token>> tokenList = lexer.tokenise();
    if (verboseFlag) {printTokens(tokenList);} // print tokens
    Parser parser(tokenList);
    std::unique_ptr<Node> nodeTree;
    do {
        nodeTree = parser.parse();
        if (nodeTree) {  // only process non-null nodes
            if (nodeTree->getType() == NodeType::EndOfFile) {
                break; // exit if we get an EndOfFile node
            }
            if (verboseFlag) {std::cout << *nodeTree << std::endl << std::endl;} // print node
            std::unique_ptr<Literal> returnLiteral = visit(nodeTree, &globalContext);
            if (verboseFlag) { if (returnLiteral) {
                std::cout << *returnLiteral << std::endl << std::string(100, '-') << std::endl;
            } } // print visited literal return
        }
    }
    while (true);
}

std::unique_ptr<Literal> Interpreter::visit(const std::unique_ptr<Node> &node, Context *context) {
    switch (node->getType()) {
        case NodeType::Number:
            return visitNumberNode(dynamic_cast<Number*>(node.get()), context);
        case NodeType::String:
            return visitStringNode(dynamic_cast<StringNode*>(node.get()), context);
        case NodeType::BinaryOperator:
            return visitBinaryOpNode(dynamic_cast<BinaryOperator*>(node.get()), context);
        case NodeType::UnaryOperator:
            return visitUnaryOpNode(dynamic_cast<UnaryOperator*>(node.get()), context);
        case NodeType::VarAccess:
            return visitVarAccessNode(dynamic_cast<VarAccess*>(node.get()), context);
        case NodeType::VarAssgnment:
            return visitVarAssignNode(dynamic_cast<VarAssignment*>(node.get()), context);
        case NodeType::VarIncrement:
            return visitVarIncrementNode(dynamic_cast<VarIncrement*>(node.get()), context);
        case NodeType::VarDecrement:
            return visitVarDecrementNode(dynamic_cast<VarDecrement*>(node.get()), context);
        case NodeType::LibCall:
            return visitLibCallNode(dynamic_cast<LibCall*>(node.get()), context);
        case NodeType::IfStmt:
            return visitIfStmtNode(dynamic_cast<IfStmt*>(node.get()), context);
        case NodeType::WhileStmt:
            return visitWhileStmtNode(dynamic_cast<WhileStmt*>(node.get()), context);
        case NodeType::ForStmt:
            return visitForStmtNode(dynamic_cast<ForStmt*>(node.get()), context);
        case NodeType::FuncDef:
            return visitFuncDefNode(dynamic_cast<FuncDef*>(node.get()), context);
        case NodeType::FuncCall:
            return visitFuncCallNode(dynamic_cast<FuncCall*>(node.get()), context);
        case NodeType::ReturnCall:
            return visitReturnCallNode(dynamic_cast<ReturnCall*>(node.get()), context);
        default:
            throw VisRunTimeError("visit node method not defined");
    }
}

std::unique_ptr<Literal> Interpreter::visitNumberNode(const Number* node, Context* context) {
    const Token token = node->getToken();
    const TokenType type = token.getType();
    std::unique_ptr<Literal> numberLiteral = nullptr;
    if (type == TokenType::INT) {
        const int value = std::get<int>(token.getValue());
        numberLiteral = std::make_unique<IntLiteral>(value);
    }
    else if (type == TokenType::FLOAT) {
        const float value = std::get<float>(token.getValue());
        numberLiteral = std::make_unique<FloatLiteral>(value);
    }
    else {
        throw VisRunTimeError("When visiting number node was provided token of type <" + tokenTypeToStr(type) +
            "> instead of INT or FLOAT");
    }
    numberLiteral->setPosition(token.getPos());
    numberLiteral->setContext(context);
    return numberLiteral;
}

std::unique_ptr<Literal> Interpreter::visitStringNode(const StringNode* node, Context* context) {
    const Token token = node->getToken();
    const std::string value = std::get<std::string>(token.getValue());
    std::unique_ptr<Literal> stringLiteral = std::make_unique<StringLiteral>(value);
    stringLiteral->setPosition(token.getPos());
    stringLiteral->setContext(context);
    return stringLiteral;
}

std::unique_ptr<Literal> Interpreter::visitBinaryOpNode(const BinaryOperator *node, Context *context) {
    const Token token = node->getToken();
    const std::unique_ptr<Node> &left = node->getLeftNode();
    const Operator operatorNode = node->getOperatorNode();
    const std::unique_ptr<Node> &right = node->getRightNode();
    const std::unique_ptr<Literal> leftvalue = visit(left, context);
    const std::unique_ptr<Literal> rightvalue = visit(right, context);
    std::unique_ptr<Literal> uniqueLiteral;
    switch (operatorNode.getToken().getType()) {
        case TokenType::PLUS:
            uniqueLiteral = leftvalue->add(*rightvalue);
            break;
        case TokenType::MINUS:
            uniqueLiteral = leftvalue->subtract(*rightvalue);
            break;
        case TokenType::MUL:
            uniqueLiteral = leftvalue->multiply(*rightvalue);
            break;
        case TokenType::DIV:
            uniqueLiteral = leftvalue->divide(*rightvalue);
            break;
        case TokenType::MOD:
            uniqueLiteral = leftvalue->modulo(*rightvalue);
            break;
        case TokenType::TRUEEQUALS:
            uniqueLiteral = leftvalue->compareTE(*rightvalue);
            break;
        case TokenType::NOTEQUAL:
            uniqueLiteral = leftvalue->compareNE(*rightvalue);
            break;
        case TokenType::LESSTHAN:
            uniqueLiteral = leftvalue->compareLT(*rightvalue);
            break;
        case TokenType::LESSEQUAL:
            uniqueLiteral = leftvalue->compareLTE(*rightvalue);
            break;
        case TokenType::GREATERTHAN:
            uniqueLiteral = leftvalue->compareGT(*rightvalue);
            break;
        case TokenType::GREATEREQUAL:
            uniqueLiteral = leftvalue->compareGTE(*rightvalue);
            break;
        case TokenType::KEYWORD:
            if (operatorNode.getToken().matches(TokenType::KEYWORD, "and")) {
                uniqueLiteral = leftvalue->andWith(*rightvalue);
                break;
            }
            else if (operatorNode.getToken().matches(TokenType::KEYWORD, "or")) {
                uniqueLiteral = leftvalue->orWith(*rightvalue);
                break;
            }
        default:
            throw ParseError("did not recognise token <"
                + tokenTypeToStr(operatorNode.getToken().getType())
                + "> inside binary opertaion instead expected: PLUS, MINUS, MUL, DIV");
    }
    uniqueLiteral->setPosition(token.getPos());
    uniqueLiteral->setContext(context);
    return uniqueLiteral;
}

std::unique_ptr<Literal> Interpreter::visitUnaryOpNode(const UnaryOperator* node, Context* context) {
    const Token token = node->getToken();
    const Operator operatorNode = node->getOperator();
    const std::unique_ptr<Literal> valueLiteral = visit(node->getValue(), context);
    std::unique_ptr<Literal> returnLiteral;
    if(operatorNode.getToken().getType() == TokenType::MINUS) {
        returnLiteral = valueLiteral->multiply(IntLiteral(-1));
    }
    else if (operatorNode.getToken().matches(TokenType::KEYWORD, "not")) {
        returnLiteral = valueLiteral->notSelf();
    }
    else {
        throw ParseError("unknown operator <"
                + tokenTypeToStr(operatorNode.getToken().getType())
                + "> for unary operation, expected MINUS or KEYWORD<not>");
    }
    returnLiteral->setPosition(token.getPos());
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> Interpreter::visitVarAssignNode(const VarAssignment *node, Context *context) {
    const std::string varName = std::get<std::string>(node->getToken().getValue());
    auto literalValue = std::unique_ptr(visit(node->getValue(), context));
    std::unique_ptr<Literal> clonedValue = literalValue->clone();
    context->getSymbolTable().set(varName, std::move(literalValue));
    return clonedValue;
}

std::unique_ptr<Literal> Interpreter::visitVarAccessNode(const VarAccess *node, Context* context) {
    const std::string varName = std::get<std::string>(node->getToken().getValue());
    std::unique_ptr<Literal> value = context->getSymbolTable().getLiteral(varName)->clone();
    if (not value) {
        throw VisRunTimeError("unknown variable " + varName);
    }
    return value;
}

std::unique_ptr<Literal> Interpreter::visitVarIncrementNode(const VarIncrement *node, Context* context) {
    const std::string varName = std::get<std::string>(node->getToken().getValue());
    std::unique_ptr<Literal> value = context->getSymbolTable().getLiteral(varName)->clone();
    if (not value) {throw VisRunTimeError("unknown variable " + varName);}
    value = value->add(IntLiteral(1));
    std::unique_ptr<Literal> clonedValue = value->clone();
    context->getSymbolTable().set(varName, std::move(value));
    return clonedValue;
}

std::unique_ptr<Literal> Interpreter::visitVarDecrementNode(const VarDecrement *node, Context* context) {
    const std::string varName = std::get<std::string>(node->getToken().getValue());
    std::unique_ptr<Literal> value = context->getSymbolTable().getLiteral(varName)->clone();
    if (not value) {throw VisRunTimeError("unknown variable " + varName);}
    value = value->subtract(IntLiteral(1));
    std::unique_ptr<Literal> clonedValue = value->clone();
    context->getSymbolTable().set(varName, std::move(value));
    return clonedValue;
}

std::unique_ptr<Literal> Interpreter::visitLibCallNode(const LibCall *node, Context *context) {
    const std::string &libFunc = std::get<std::string>(node->getToken().getValue());
    if (libFunc == "out") {
        for (const auto& argumentNode : node->getArgumentNodes()) {
            if (const std::unique_ptr<Literal> returnLiteral = visit(argumentNode, context)) {
                std::cout << returnLiteral->getStringValue() << " ";
            }
        }
        std::cout << std::endl;
    } else {
        throw VisRunTimeError("libCall was made to an unknown function: " + libFunc);
    }
    return nullptr;
}

std::unique_ptr<Literal> Interpreter::visitIfStmtNode(const IfStmt* node, Context* context) {
    std::unique_ptr<Literal> comparisonResult = visit(node->getComparison(), context);
    const std::vector<std::unique_ptr<Node>>& executableNodes =
        comparisonResult->getBoolValue() ? node->getIfBlock() : node->getElseBlock();
    for (const std::unique_ptr<Node>& executableNode : executableNodes) {
        visit(executableNode, context);
    }
    return comparisonResult;
}

std::unique_ptr<Literal> Interpreter::visitWhileStmtNode(const WhileStmt* node, Context* context) {
    std::unique_ptr<Literal> comparisonResult = visit(node->getComparison(), context);
    const std::vector<std::unique_ptr<Node>>& executableNodes = node->getWhileBlock();
    while (visit(node->getComparison(), context)->getBoolValue()) {
        for (const std::unique_ptr<Node>& executableNode : executableNodes) {
            visit(executableNode, context);
        }
    }
    return comparisonResult;
}

std::unique_ptr<Literal> Interpreter::visitForStmtNode(const ForStmt* node, Context* context) {
    visit(node->getVarDeclare(), context);
    std::unique_ptr<Literal> comparisonResult = visit(node->getCondition(), context);
    const std::vector<std::unique_ptr<Node>>& executableNodes = node->getForBlock();
    while (visit(node->getCondition(), context)->getBoolValue()) {
        for (const std::unique_ptr<Node>& executableNode : executableNodes) {
            visit(executableNode, context);
        }
        visit(node->getStep(), context);
    }
    return comparisonResult;
}

std::unique_ptr<Literal> Interpreter::visitFuncDefNode(const FuncDef* node, Context* context) {
    auto contextForFunc = std::make_unique<Context>(node->getName());
    contextForFunc->setParentContext(context);
    contextForFunc->setSymbolTable(SymbolTable(&context->getSymbolTable()));
    // Clone arguments and body
    std::vector<Token> clonedArgs;
    for (const Token& token : node->getArguments()) {clonedArgs.push_back(token.clone());}
    std::vector<std::unique_ptr<Node>> clonedBody = Node::cloneNodeVector(node->getFunctionBody());
    std::unique_ptr<FunctionLiteral> funcLiteral = std::make_unique<FunctionLiteral>(
        node->getName(),
        std::move(clonedArgs),
        std::move(clonedBody),
        std::move(contextForFunc)
        );
    funcLiteral->setContext(context);
    funcLiteral->setPosition(node->getToken().getPos());
    std::cout << *funcLiteral->getScopeContext() << std::endl;
    const std::string funcName = funcLiteral->getName();
    auto clonedFunc = funcLiteral->clone();
    clonedFunc->setContext(context);
    clonedFunc->setPosition(node->getToken().getPos());
    context->getSymbolTable().set(funcName, std::move(funcLiteral));
    return clonedFunc;
}

std::unique_ptr<Literal> Interpreter::visitFuncCallNode(const FuncCall* node, Context* context) {
    const std::string name = node->getName();
    Literal* func = context->getSymbolTable().getLiteral(name);
    const FunctionLiteral* funcLiteral = dynamic_cast<FunctionLiteral*>(func);
    if (!funcLiteral) {
        throw VisRunTimeError("function >>> " + name + " <<< called but does not point to a function");
    }
    const std::unique_ptr<Literal> funcClone = funcLiteral->clone();
    FunctionLiteral* concreteFunc = dynamic_cast<FunctionLiteral*>(funcClone.get());
    concreteFunc->setContext(funcLiteral->getContext());
    concreteFunc->setPosition(funcLiteral->getPosition());
    const auto& funcArgs = funcLiteral->getArgs();
    const auto& passedArgs = node->getArguments();
    if (funcArgs.size() != passedArgs.size()) {
        throw VisRunTimeError("function >>> " + name + " <<< was called with incorrect arguments");
    }
    for (int i = 0; i < funcArgs.size(); i++) {
        std::string argName = std::get<std::string>(funcArgs[i].getValue());
        std::unique_ptr<Literal> value = visit(passedArgs[i], context);
        if (!value) {throw InterpretError("function argument evaluated to a null ptr");}
        concreteFunc->getScopeContext()->getSymbolTable().set(argName, std::move(value));
    }
    try {
        for (const std::unique_ptr<Node>& bodyNode : funcLiteral->getBody()) {
            visit(bodyNode, concreteFunc->getScopeContext().get());
        }
    }
    catch (ReturnSignal& returnSignal) {
        return returnSignal.getValue();
    }
    return nullptr;
}

std::unique_ptr<Literal> Interpreter::visitReturnCallNode(const ReturnCall* node, Context* context) {
    std::unique_ptr<Literal> returnValue = visit(node->getExpression(), context);
    throw ReturnSignal(std::move(returnValue));
}




