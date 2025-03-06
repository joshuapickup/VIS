//
// Created by joshu on 06/03/2025.
//

#ifndef CONTEXT_H
#define CONTEXT_H


#include "lexer.h"

class SymbolTable {
public:
    SymbolTable();
    ValueLiteral get(const std::string &name);
    void set(const std::string& name, const ValueLiteral& value);
    void remove(const std::string &name);
private:
    SymbolTable *parentSymbolTable;
    std::unordered_map<std::string, ValueLiteral> table;
};

class Context {
public:
    Context();
    [[nodiscard]] SymbolTable* getSymbolTable() const;
private:
    // auto parent;
    // auto entryPos
    // auto displayName
    SymbolTable* symbolTable;
};




#endif //CONTEXT_H
