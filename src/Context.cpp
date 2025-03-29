//
// Created by joshu on 06/03/2025.
//

#include "Context.h"

#include <ostream>
#include <utility>
#include "Error.h"
#include "Literal.h"


//SYMBOL TABLE DEFINITION
SymbolTable::SymbolTable() : parentSymbolTable(nullptr), table() {}

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

std::ostream& operator<<(std::ostream& os, const SymbolTable& table) {
    for (const auto& [name, literal] : table.table) {
        os << name << std::endl;
        if (literal) os << *literal;
        else os << "REFERENCE WAS NULL";
        os << "\n";
    }
    return os;
}



//CONTEXT DEFINITION
Context::Context(std::string displayName, Context* parentContext, std::map<std::string, std::string> entryPos) :
diplayName(std::move(displayName)),
parentContext(parentContext),
entryPoint(std::move(entryPos)),
symbolTable(SymbolTable()) {

}

SymbolTable& Context::getSymbolTable() {return symbolTable;}

void Context::setSymbolTable(SymbolTable&& symbolTable) {this->symbolTable = std::move(symbolTable);}

void Context::setParentContext(Context *context) {this->parentContext = context;}

std::string Context::getDisplayName() {return diplayName;}

std::map<std::string, std::string> Context::getEntryPoint() {return entryPoint;}

void Context::setEntryPoint(const std::map<std::string, std::string> &pos) { entryPoint = pos;}

std::ostream& operator<<(std::ostream& os, const Context& context) {
    os << "Context <" << context.diplayName << ">" << std::endl
    << context.symbolTable;
    return os;
}

