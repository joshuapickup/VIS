//
// Created by joshu on 05/03/2025.
//

#ifndef LITERAL_H
#define LITERAL_H

#include "Context.h"

class Literal {
public:
    Literal();
    Context* getContext();
    void setContext(Context* context);
private:
    Context* context;
};

class NumberLiteral : public Literal{
public:
    NumberLiteral();
    NumberLiteral* add(NumberLiteral*);
    NumberLiteral* subtract(NumberLiteral*);
    NumberLiteral* multipy(NumberLiteral*);
    NumberLiteral* divide(NumberLiteral*);
private:
};

class IntLiteral : public NumberLiteral{
public:
    IntLiteral();
private:

};

class FloatLiteral : public NumberLiteral{
public:
    FloatLiteral();
private:
};


#endif //LITERAL_H
