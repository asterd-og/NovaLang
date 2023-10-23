#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include "token.hpp"

class Lexer {
    public:
    std::string data;
    char c;
    int pos, line, col;
    std::string value;
    TokType type;
    std::vector<Token> tokens;
    Lexer(std::string data);
    void peek();
    std::vector<Token> lex();
};