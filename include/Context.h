//
// Created by joshu on 06/03/2025.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_map>
#include "lexer.h"
#include "Literal.h"

class SymbolTable {
public:
    SymbolTable();
    Literal* get(const std::string &name);
    void set(const std::string& name, const Literal* value);
    void remove(const std::string &name);
private:
    SymbolTable *parentSymbolTable;
    std::unordered_map<std::string, Literal> table;
};



class Context {
public:
    explicit Context(std::string displayName,
                Context* parentContext = nullptr,
                std::map<std::string, std::string> entryPos = PositionHandler::nullPos);

    [[nodiscard]] SymbolTable* getSymbolTable() const;
    void setSymbolTable(SymbolTable* symbolTable);
    std::string getDisplayName();
    std::map<std::string, std::string> getEntryPoint();
private:
    std::string diplayName;
    Context* parentContext;
    std::map<std::string, std::string> entryPoint;
    SymbolTable* symbolTable;
};


#endif //CONTEXT_H
