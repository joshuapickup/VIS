//
// Created by joshu on 06/03/2025.
//

#include "Context.h"

#include <utility>
#include "Error.h"


//SYMBOL TABLE DEFINITION
SymbolTable::SymbolTable() : parentSymbolTable(nullptr), table(){

}

Literal* SymbolTable::getLiteral(const std::string &name) {
    if (const auto it = table.find(name); it != table.end()) {
        return it->second.get();
    }
    if (parentSymbolTable) {
        return parentSymbolTable->getLiteral(name);
    }
    throw VisRunTimeError("symbol '" + name + "' was not found in lookup tables");
}

void SymbolTable::set(const std::string& name, std::unique_ptr<Literal> value) {
    table[name] = std::move(value);
}


void SymbolTable::remove(const std::string& name) {table.erase(name);}


//CONTEXT DEFINITION
Context::Context(std::string displayName, Context* parentContext, std::map<std::string, std::string> entryPos) :
diplayName(std::move(displayName)),
parentContext(parentContext),
entryPoint(std::move(entryPos)),
symbolTable(SymbolTable()) {

}

SymbolTable& Context::getSymbolTable() {return symbolTable;}

void Context::setSymbolTable(SymbolTable&& symbolTable) {this->symbolTable = std::move(symbolTable);}

std::string Context::getDisplayName() {return diplayName;}

std::map<std::string, std::string> Context::getEntryPoint() {return entryPoint;}

void Context::setEntryPoint(const std::map<std::string, std::string> &pos) { entryPoint = pos;}

