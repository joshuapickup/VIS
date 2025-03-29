//
// Created by joshu on 05/03/2025.
//

#include <iostream>
#include <cmath>
#include <utility>


#include "Context.h"
#include "Error.h"
#include "Literal.h"
#include "PositionHandler.h"


//LITERAL DEFINITION
Literal::Literal() : position(PositionHandler::nullPos), context(nullptr){

}

void Literal::setContext(Context* context) {
    this->context = context;
    this->context->setEntryPoint(position);
}

Context* Literal::getContext() const {return context;}

void Literal::setPosition(const std::map<std::string, std::string> &pos) {position = pos;}

std::map<std::string, std::string> Literal::getPosition() const {return position;}

std::unique_ptr<Literal> Literal::compareLT(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getNumberValue() < other.getNumberValue());
}

std::unique_ptr<Literal> Literal::compareLTE(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getNumberValue() <= other.getNumberValue());
}

std::unique_ptr<Literal> Literal::compareGT(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getNumberValue() > other.getNumberValue());
}

std::unique_ptr<Literal> Literal::compareGTE(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getNumberValue() >= other.getNumberValue());
}

std::unique_ptr<Literal> Literal::andWith(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getBoolValue() and other.getBoolValue());
}

std::unique_ptr<Literal> Literal::orWith(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getBoolValue() or other.getBoolValue());
}

std::unique_ptr<Literal> Literal::notSelf() const {
    return std::make_unique<BoolLiteral>(not getBoolValue());
}

void Literal::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Literal<" << std::endl;
    os << std::string(tabCount, '\t') << "Literal>" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Literal &literal) {
    literal.printLiteral(os, 0);
    return os;
}



//BOOL LITERAL DEFINITION
BoolLiteral::BoolLiteral(const bool value) : Literal(), value(value) {

}

double BoolLiteral::getNumberValue() const {
    if (value) {
        return 1;
    }
    return 0;
}

bool BoolLiteral::getBoolValue() const{
    return value;
};

std::string BoolLiteral::getStringValue() const {
    if (value) {
        return "true";
    }
    return "false";
}

std::unique_ptr<Literal> BoolLiteral::add(const Literal& other) const {
    if (const auto* otherBool = dynamic_cast<const BoolLiteral*>(&other)) {
        return std::make_unique<BoolLiteral>(this->getBoolValue() || otherBool->getBoolValue());
    }
    else {
        throw VisRunTimeError("cannot add a non boolean value to a boolean value");
    }
}

std::unique_ptr<Literal> BoolLiteral::subtract(const Literal& other) const {
    throw VisRunTimeError("cannot subtract with a bool");
}

std::unique_ptr<Literal> BoolLiteral::multiply(const Literal& other) const {
    throw VisRunTimeError("cannot multiply with a bool");
}

std::unique_ptr<Literal> BoolLiteral::divide(const Literal& other) const {
    throw VisRunTimeError("cannot divide with a bool");
}

std::unique_ptr<Literal> BoolLiteral::compareTE(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getBoolValue() == other.getBoolValue());
}

std::unique_ptr<Literal> BoolLiteral::compareNE(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getBoolValue() != other.getBoolValue());
}

std::unique_ptr<Literal> BoolLiteral::clone() const {return std::make_unique<BoolLiteral>(*this);}

void BoolLiteral::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "BoolLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Value: " << getStringValue() << std::endl;
    os << std::string(tabCount+1, '\t') <<"Position: {line: " << position.at("line");
    os << " | Pos:" << position.at("charPos") << "}" << std::endl;
    os << std::string(tabCount, '\t') << "BoolLiteral>" << std::endl;
}



//NUMBER LITERAL DEFINITION
NumberLiteral::NumberLiteral() : Literal(){
}

std::unique_ptr<Literal> NumberLiteral::add(const Literal& other) const {
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getNumberValue() + other.getNumberValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::subtract(const Literal& other) const {
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getNumberValue() - other.getNumberValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::multiply(const Literal& other) const {
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getNumberValue() * other.getNumberValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::divide(const Literal& other) const {
    if (other.getNumberValue() == 0) {
        throw VisRunTimeError("Division by zero!");
    }
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getNumberValue() / other.getNumberValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::compareTE(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getNumberValue() == other.getNumberValue());
}

std::unique_ptr<Literal> NumberLiteral::compareNE(const Literal& other) const {
    return std::make_unique<BoolLiteral>(getNumberValue() != other.getNumberValue());
}

void NumberLiteral::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "NumberLiteral<" << std::endl;
    os << std::string(tabCount, '\t') << "NumberLiteral>" << std::endl;
}

std::unique_ptr<NumberLiteral> NumberLiteral::makeResultLiteral(float value) {
    if (std::floor(value) == value) {
        return std::make_unique<IntLiteral>(static_cast<int>(value));
    }
    return std::make_unique<FloatLiteral>(value);
}



//INT LITERAL DEFINITION
IntLiteral::IntLiteral(const int value) : NumberLiteral(), value(value) {
}

double IntLiteral::getNumberValue() const {return value;}

bool IntLiteral::getBoolValue() const {return value != 0;}

std::string IntLiteral::getStringValue() const {return std::to_string(value);}

std::unique_ptr<Literal> IntLiteral::clone() const {return std::make_unique<IntLiteral>(*this);}

void IntLiteral::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "IntLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Value: " << value << std::endl;
    os << std::string(tabCount+1, '\t') <<"Position: {line: " << position.at("line");
    os << " | Pos:" << position.at("charPos") << "}" << std::endl;
    os << std::string(tabCount, '\t') << "IntLiteral>" << std::endl;
}



//FLOAT LITERAL DEFINITION
FloatLiteral::FloatLiteral(const float value) : NumberLiteral(), value(value){
}

double FloatLiteral::getNumberValue() const {return value;}

bool FloatLiteral::getBoolValue() const {return value != 0;}

std::string FloatLiteral::getStringValue() const {return std::to_string(value);}

std::unique_ptr<Literal> FloatLiteral::clone() const {return std::make_unique<FloatLiteral>(*this);}

void FloatLiteral::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "FloatLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Value: " << value << std::endl;
    os << std::string(tabCount+1, '\t') <<"Position: {line: " << position.at("line");
    os << " | Pos:" << position.at("charPos") << "}" << std::endl;
    os << std::string(tabCount, '\t') << "FloatLiteral>" << std::endl;
}



//FUNCTION LITERAL DEFINITION
FunctionLiteral::FunctionLiteral(
    std::string name,
    std::vector<Token> args,
    std::vector<std::unique_ptr<Node>> body,
    std::unique_ptr<Context> scope) :
Literal(),
name(std::move(name)),
argTokens(std::move(args)),
bodyNodes(std::move(body)),
scopeContext(std::move(scope)) {}

std::string FunctionLiteral::getName() const {return name;}

const std::vector<Token>& FunctionLiteral::getArgs() const {return argTokens;}

const std::vector<std::unique_ptr<Node>>& FunctionLiteral::getBody() const {return bodyNodes;}

std::unique_ptr<Literal> FunctionLiteral::add(const Literal &other) const {
    throw VisRunTimeError("cannot add a function");
}

std::unique_ptr<Literal> FunctionLiteral::subtract(const Literal &other) const {
    throw VisRunTimeError("cannot subtract a function");
}

std::unique_ptr<Literal> FunctionLiteral::multiply(const Literal &other) const {
    throw VisRunTimeError("cannot multiply a function");
}

std::unique_ptr<Literal> FunctionLiteral::divide(const Literal &other) const {
    throw VisRunTimeError("cannot divide a function");
}

std::unique_ptr<Literal> FunctionLiteral::compareTE(const Literal &other) const {
    throw VisRunTimeError("cannot compare a function");
}

std::unique_ptr<Literal> FunctionLiteral::compareNE(const Literal &other) const {
    throw VisRunTimeError("cannot compare a function");
}

double FunctionLiteral::getNumberValue() const {
    throw VisRunTimeError("function contains no value");
}

bool FunctionLiteral::getBoolValue() const {
    throw VisRunTimeError("function contains no value");
}

std::string FunctionLiteral::getStringValue() const {
    throw VisRunTimeError("function contains no value");
}

std::unique_ptr<Literal> FunctionLiteral::clone() const {
    std::vector<Token> clonedArgs;
    clonedArgs.reserve(this->argTokens.size());
    for (const Token& token : this->argTokens) {clonedArgs.push_back(token.clone());}
    std::vector<std::unique_ptr<Node>> clonedBody = Node::cloneNodeVector(bodyNodes);
    std::unique_ptr<Context> clonedContext;
    if (scopeContext) {clonedContext = scopeContext->clone();}
    else {clonedContext = nullptr;}
    return std::make_unique<FunctionLiteral>(
        name,
        std::move(clonedArgs),
        std::move(clonedBody),
        std::move(clonedContext)
    );
}

void FunctionLiteral::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "FunctionLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Name: " << name << std::endl;
    os << std::string(tabCount+1, '\t') <<"Declared on line: " << stoi(position.at("line"))+1 << std::endl;
    os << std::string(tabCount, '\t') << "FunctionLiteral>" << std::endl;
}


