//
// Created by joshu on 06/03/2025.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_map>
#include <memory>
#include "lexer.h"


class Literal; // decleration to allow use of context without circular loop

class SymbolTable {
public:
    SymbolTable();
    Literal* getLiteral(const std::string &name);
    void set(const std::string& name, std::unique_ptr<Literal> value);
    void remove(const std::string &name);
    friend std::ostream& operator<<(std::ostream& os, const SymbolTable& table);
private:
    SymbolTable *parentSymbolTable;
    std::unordered_map<std::string, std::unique_ptr<Literal>> table;
};



class Context {
public:
    explicit Context(std::string displayName,
                Context* parentContext = nullptr,
                std::map<std::string, std::string> entryPos = PositionHandler::nullPos);
    [[nodiscard]] SymbolTable& getSymbolTable();
    void setSymbolTable(SymbolTable&& symbolTable);
    void setParentContext(Context* context);
    std::string getDisplayName();
    std::map<std::string, std::string> getEntryPoint();
    void setEntryPoint(const std::map<std::string, std::string> &pos);
    friend std::ostream& operator<<(std::ostream& os, const Context& context);
private:
    std::string diplayName;
    Context* parentContext;
    std::map<std::string, std::string> entryPoint;
    SymbolTable symbolTable;
};


#endif //CONTEXT_H
