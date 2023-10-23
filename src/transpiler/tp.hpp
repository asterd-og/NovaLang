#pragma once

#include <stdint.h>
#include "../parser/parser.hpp"
#include "../lexer/lexer.hpp"

#include <vector>
#include <map>
#include <string>

std::string stringf(char* str, ...);

class Tp {
    public:
        std::vector<std::string> gVars;              // Global vars
        std::map<std::string, std::vector<std::string>> sVars;    // Scope vars

        std::vector<Node> ast;

        std::string code;

        bool insideScope;
        std::string scope;

        Node node;

        Token getArg(int pos);

        void evalVarDecl();
        void evalFunDecl();
        void evalFunCall();
        void evalRet();

        std::string transpile(std::vector<Node> ast);
};