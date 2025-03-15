#include <iostream>
#include <fstream>
#include <string>

#include "PositionHandler.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

//TODO implement logic inside interpreter calls and call Inrepreter::interpretFile() or Inrepreter::interpretConsole()
int runFile(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }
    PositionHandler positionHandler(filename);
    SymbolTable globalSymbolTable = SymbolTable();
    globalSymbolTable.set("null", std::make_unique<IntLiteral>(0));
    Context globalContext = Context(filename);
    globalContext.setSymbolTable(std::move(globalSymbolTable));
    Lexer lexer(positionHandler);
    Interpreter interpreter = Interpreter();
    std::string line;

    while (std::getline(inputFile, line)) {
        positionHandler.advanceLine(line);
        std::vector<Token> tokenList = lexer.tokenise();
        // print Tokens
        for (Token &token : tokenList) { std::cout << token << std::endl;} std::cout << std::endl;
        Parser parser = Parser(tokenList);
        std::unique_ptr<Node> nodeTree = parser.parse();
        // print Abstract Syntax Tree
        if (nodeTree) {
            //std::cout << *nodeTree << std::endl;
            ;
        }
        else {continue;}

        if (std::unique_ptr<Literal> literalVal = interpreter.visit(nodeTree, &globalContext)) {
            std::cout << *literalVal << std::endl;
        }
        std::cout << std::string(80, '-') << std::endl;
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
