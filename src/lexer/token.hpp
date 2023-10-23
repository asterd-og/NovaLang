#pragma once

#include <stdint.h>
#include <string>

typedef enum {
    eof,
    name,
    str,
    num,
    type,

    eq,
    semi,
    LPar,
    RPar,
    LBrac,
    RBrac,
    eqeq,
    comma,

    ifkyd,
    elsekyd,
    elseifkyd,
    inckyd,
    retkyd,
} TokType;

static const char* tok2str[] = {
    "eof",
    "name",
    "str",
    "num",
    "type",

    "eq",
    "semi",
    "LPar",
    "RPar",
    "LBrac",
    "RBrac",
    "eqeq",
    "comma",

    "ifkyd",
    "elsekyd",
    "elseifkyd",
    "inckyd",
    "retkyd",
};

class Token {
    public:
    TokType type;
    std::string value;
    int line;
    int col;

    inline Token(TokType type = TokType::eof, std::string value = "", int line = 0, int col = 0) {
        this->type = type;
        this->value = value;
        this->line = line;
        this->col = col;
    }
};