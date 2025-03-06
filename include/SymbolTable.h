//
// Created by joshu on 31/01/2025.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include <string>
#include <any>
#include <optional>


class SymbolTable {
    public:
        SymbolTable(SymbolTable* parentTablePtr = nullptr);
        std::optional<std::any> get(std::string name);
        void set(const std::string &name, std::any value);
        void remove(std::string name);
        ~SymbolTable();
    private:
        SymbolTable* parentTable;
        std::map<std::string, std::any> symbolDict;

};



#endif //SYMBOLTABLE_H
