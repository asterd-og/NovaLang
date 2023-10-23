#pragma once

#include <stdint.h>
#include <vector>
#include "../lexer/token.hpp"
#include "node.hpp"

class Parser {
    public:
        std::vector<Node> ast;
        std::vector<Token> tokens;
        int pos;

        NodeType type;
        Token left;
        Token mid;
        Token right;
        std::vector<Node> innerNodes;
        
        Parser(std::vector<Token> tokens);

        Token eat(TokType type);
        Token consume();
        Token get();

        std::vector<Node> parseBody();

        Node parseVarDecl();
        Node parseFunDecl();

        Node parseFunCall();
        Node parseFunCallNA(); // No Args

        Node parseRet();

        Node parseDecl();
        Node parseStmt();
        Node parseExpr();

        void unexpectedError();

        void parse();
};