//
// Created by joshu on 05/03/2025.
//

#include <iostream>
#include <cmath>


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

std::map<std::string, std::string> Literal::getPosition() {return position;}

void Literal::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "Literal<" << std::endl;
    os << std::string(tabCount, '\t') << "Literal>" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Literal &literal) {
    literal.printLiteral(os, 0);
    return os;
}


//NUMBER LITERAL DEFINITION
NumberLiteral::NumberLiteral() :Literal(){
}

std::unique_ptr<Literal> NumberLiteral::add(const Literal& other) const {
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getValue() + other.getValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::subtract(const Literal& other) const {
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getValue() - other.getValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::multiply(const Literal& other) const {
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getValue() * other.getValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
}

std::unique_ptr<Literal> NumberLiteral::divide(const Literal& other) const {
    if (other.getValue() == 0) {
        throw VisRunTimeError("Division by zero!");
    }
    std::unique_ptr<NumberLiteral> returnLiteral = makeResultLiteral(getValue() / other.getValue());
    returnLiteral->setPosition(position);
    returnLiteral->setContext(context);
    return returnLiteral;
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

double IntLiteral::getValue() const {return value;}

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

double FloatLiteral::getValue() const {return value;}

std::unique_ptr<Literal> FloatLiteral::clone() const {return std::make_unique<FloatLiteral>(*this);}

void FloatLiteral::printLiteral(std::ostream &os, const int tabCount) const {
    os << std::string(tabCount, '\t') << "FloatLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Value: " << value << std::endl;
    os << std::string(tabCount+1, '\t') <<"Position: {line: " << position.at("line");
    os << " | Pos:" << position.at("charPos") << "}" << std::endl;
    os << std::string(tabCount, '\t') << "FloatLiteral>" << std::endl;
}