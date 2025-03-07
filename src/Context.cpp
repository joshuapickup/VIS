//
// Created by joshu on 06/03/2025.
//

#include "Context.h"

#include <utility>
#include "Error.h"


//SYMBOL TABLE DEFINITION
SymbolTable::SymbolTable() : parentSymbolTable(nullptr){
    table = {};
}

Literal* SymbolTable::get(const std::string &name) {
    auto it = table.find(name);
    if (it != table.end()) {
        return &it->second;
    }
    if (parentSymbolTable) {
        return parentSymbolTable->get(name);
    }
    throw VisRunTimeError("symbol '" + name + "' was not found in lookup tables");
}

void SymbolTable::set(const std::string& name, const Literal* value) {
    table.emplace(name, *value);
}

void SymbolTable::remove(const std::string& name) {
    table.erase(name);
}


//CONTEXT DEFINITION
Context::Context(std::string displayName, Context* parentContext, std::map<std::string, std::string> entryPos) :
diplayName(std::move(displayName)),
parentContext(parentContext),
entryPoint(std::move(entryPos)),
symbolTable(nullptr) {

}

SymbolTable* Context::getSymbolTable() const {return symbolTable;}

void Context::setSymbolTable(SymbolTable* symbolTable) {this->symbolTable = symbolTable;}

std::string Context::getDisplayName() {return diplayName;}

std::map<std::string, std::string> Context::getEntryPoint() {return entryPoint;}

