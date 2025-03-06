//
// Created by joshu on 06/03/2025.
//

#include "Context.h"
#include "Error.h"


//SYMBOL TABLE DEFINITION
SymbolTable::SymbolTable() : parentSymbolTable(nullptr){
    table = {};
}

ValueLiteral SymbolTable::get(const std::string &name) {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second;
    }
    if (parentSymbolTable) {
        return parentSymbolTable->get(name);
    }
    throw VisRunTimeError("symbol '" + name + "' was not found in lookup tables");
}

void SymbolTable::set(const std::string& name, const ValueLiteral& value) {
    table.emplace(name, value);
}

void SymbolTable::remove(const std::string& name) {
    table.erase(name);
}


//CONTEXT DEFINITION
Context::Context() : symbolTable(nullptr) {

}

SymbolTable *Context::getSymbolTable() const {return symbolTable;}
