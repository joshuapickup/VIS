//
// Created by joshu on 31/01/2025.
//

#include <utility>

#include "../include/SymbolTable.h"

SymbolTable::SymbolTable(SymbolTable* parentTablePtr) : parentTable(parentTablePtr) {}

std::optional<std::any> SymbolTable::get(std::string name) {
    auto symbol = symbolDict.find(name);
    if (symbol != symbolDict.end()) {
        return symbol->second;
    }
    // if not found check parent symbol table (if exists)
    if (parentTable) {return parentTable->get(name);}
    return nullptr; // key not found
}

void SymbolTable::set(const std::string &name, std::any value) {
    symbolDict[name] = std::move(value);
}

void SymbolTable::remove(std::string name) {
    symbolDict.erase(name);
}
