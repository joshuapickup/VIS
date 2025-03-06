//
// Created by joshu on 05/03/2025.
//

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


//NUMBER LITERAL DEFINITION
NumberLiteral::NumberLiteral() :Literal(){
}

NumberLiteral * NumberLiteral::add(NumberLiteral *) {
}

NumberLiteral * NumberLiteral::subtract(NumberLiteral *) {
}

NumberLiteral * NumberLiteral::multipy(NumberLiteral *) {
}

NumberLiteral * NumberLiteral::divide(NumberLiteral *) {
}


//INT LITERAL DEFINITION
IntLiteral::IntLiteral() : NumberLiteral(){
}


//FLOAT LITERAL DEFINITION
FloatLiteral::FloatLiteral() : NumberLiteral(){
}
