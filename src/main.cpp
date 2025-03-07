#include <iostream>
#include <fstream>
#include <string>

#include "PositionHandler.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"


int runFile(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }
    PositionHandler positionHandler(filename);
    SymbolTable globalSymbolTable = SymbolTable();
    IntLiteral nullLiteral = IntLiteral(0);
    globalSymbolTable.set("null", &nullLiteral);
    Context globalContext = Context(filename);
    globalContext.setSymbolTable(&globalSymbolTable);
    Lexer lexer(positionHandler);
    Interpreter interpreter = Interpreter();
    std::string line;

    while (std::getline(inputFile, line)) {
        positionHandler.advanceLine(line);
        std::vector<Token> tokenList = lexer.tokenise();
        // print Tokens
        for (Token &token : tokenList) { std::cout << token << std::endl;}
        std::cout << std::endl;
        Parser parser = Parser(tokenList);
        std::unique_ptr<Node> nodeTree = parser.parse();
        if (nodeTree) { // print Abstract Syntax Tree
            std::cout << *nodeTree << std::endl;
        }
        else {
            continue;
        }
        interpreter.visit(nodeTree, &globalContext);
        std::cout << std::string(40, '-') << std::endl;
    }
    inputFile.close();
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { // check filename argument is given
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    else {
        return runFile(argv[1]);
    }
}
