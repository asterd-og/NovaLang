#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "transpiler/tp.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

void printAst(std::vector<Node> ast) {
    for (auto node : ast) {
        if (node.type == NodeType::varDecl) {
            printf("VarDecl: %s %s = %s\n",
                   node.left.value.c_str(),
                   node.mid.value.c_str(),
                   node.right.value.c_str());
        } else if (node.type == NodeType::funCall) {
            printf("FunCall: %s\nArgs:\n", node.left.value.c_str());
            for (auto inode : node.innerNodes) {
                printf("%s - %s\n", inode.left.value.c_str(),
                       tok2str[inode.left.type]);
            }
        } else if (node.type == NodeType::funDecl) {
            printf("FunDecl: %s\nBody:\n", node.mid.value.c_str());
            printAst(node.innerNodes);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Error: no output files.\n");
        std::exit(1);
    }
    
    std::ifstream file(argv[1]);

    std::stringstream buffer;
    buffer << file.rdbuf();

    printf("%s\n", buffer.str().c_str());

    Lexer lexer(buffer.str());

    std::vector<Token> toks = lexer.lex();
    /*for (auto tok : toks) {
        printf("%s - %s\n", tok.value.c_str(), tok2str[tok.type]);
    }*/

    Parser parser(toks);
    parser.parse();

    /*printf("\nNODES:\n\n");
    printAst(parser.ast);
    printf("\n");*/

    Tp tp;
    std::string code = tp.transpile(parser.ast);
    //printf("%s\n", code.c_str());

    std::ofstream out("tmp.c");
    out << code;
    out.close();

    system(stringf("clang tmp.c -o %s -Wformat-security -Wno-implicit-int -Wno-main-return-type -Wno-implicit-function-declaration", argv[2]).c_str());

    remove("tmp.c");

    //Eval evaluator;
    //evaluator.eval(parser.ast);
}