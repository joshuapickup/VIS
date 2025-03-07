//
// Created by joshu on 05/03/2025.
//

#include <iostream>

#include "Literal.h"


//LITERAL DEFINITION
Literal::Literal() : context(nullptr){

}

Context* Literal::getContext() {
    return context;
}

void Literal::setContext(Context* context) {
    this->context = context;
}

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

NumberLiteral * NumberLiteral::add(NumberLiteral *) {
    return nullptr;
}

NumberLiteral * NumberLiteral::subtract(NumberLiteral *) {
    return nullptr;
}

NumberLiteral * NumberLiteral::multipy(NumberLiteral *) {
    return nullptr;
}

NumberLiteral * NumberLiteral::divide(NumberLiteral *) {
    return nullptr;
}


void NumberLiteral::printLiteral(std::ostream &os, int tabCount) const {
    os << std::string(tabCount, '\t') << "NumberLiteral<" << std::endl;
    os << std::string(tabCount, '\t') << "NumberLiteral>" << std::endl;
}


//INT LITERAL DEFINITION
IntLiteral::IntLiteral(const int value) : NumberLiteral(), value(value) {
}

int IntLiteral::getValue() const {return value;}

void IntLiteral::printLiteral(std::ostream &os, int tabCount) const {
    os << std::string(tabCount, '\t') << "IntLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Value: " << value << std::endl;
    os << std::string(tabCount, '\t') << "IntLiteral>" << std::endl;
}


//FLOAT LITERAL DEFINITION
FloatLiteral::FloatLiteral(const float value) : NumberLiteral(), value(value){
}

float FloatLiteral::getValue() const {return value;}

void FloatLiteral::printLiteral(std::ostream &os, int tabCount) const {
    os << std::string(tabCount, '\t') << "FloatLiteral<" << std::endl;
    os << std::string(tabCount+1, '\t') << "Value: " << value << std::endl;
    os << std::string(tabCount, '\t') << "FloatLiteral>" << std::endl;
}