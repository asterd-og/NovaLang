#include "lexer.hpp"

std::map<char, TokType> stdchars = {
    {';', TokType::semi},
    {'(', TokType::LPar},
    {')', TokType::RPar},
    {'{', TokType::LBrac},
    {'}', TokType::RBrac},
    {',', TokType::comma},
};

std::map<std::string, TokType> keywords = {
    {"if", TokType::ifkyd},
    {"else", TokType::elsekyd},
    {"elseif", TokType::elseifkyd},
    {"include", TokType::inckyd},
    {"ret", TokType::retkyd},

    {"u8", TokType::type},
    {"u16", TokType::type},
    {"u32", TokType::type},
    {"u64", TokType::type},

    {"i8", TokType::type},
    {"i16", TokType::type},
    {"i32", TokType::type},
    {"i64", TokType::type},

    {"v0", TokType::type},

    {"int", TokType::type},
    {"str", TokType::type},
};

Lexer::Lexer(std::string data) {
    this->data = data;
    this->pos = 0;
    this->line = 1;
    this->col = 1;
}

void Lexer::peek() {
    if (c == '\n') {
        col = 1;
        line++;
        return;
    } else if (c == '\r') {
        col = 1;
        return;
    } else if (c == ' ' || c == '\t') {
        col++;
        return;
    } else if (std::isalpha(c) || c == '_') {
        this->value.clear();
        while (std::isalpha(c) || c == '_' || std::isdigit(c)) {
            this->value += c;
            this->pos++;
            this->col++;
            this->c = this->data[this->pos];
        }
        this->pos--;
        this->type = TokType::name;
        if (keywords.find(this->value) != keywords.end())
            this->type = keywords[this->value];
    } else if (std::isdigit(c)) {
        this->value.clear();
        while (std::isdigit(c)) {
            this->value += c;
            this->pos++;
            this->col++;
            this->c = this->data[this->pos];
        }
        this->pos--;
        this->type = TokType::num;
    } else if (c == '"') {
        this->value.clear();
        this->value += '"';
        this->pos++;
        this->c = this->data[this->pos];
        while (c != '"') {
            this->value += c;
            this->pos++;
            this->col++;
            this->c = this->data[this->pos];
        }
        this->value += '"';
        this->type = TokType::str;
    } else {
        if (stdchars.find(c) != stdchars.end()) {
            this->value = c;
            this->type = stdchars[c];
        } else {
            if (c == '=') {
                this->value = "=";
                this->type = TokType::eq;
                if (this->data[this->pos + 1] == '=') {
                    this->value = "==";
                    this->type = TokType::eqeq;
                    this->pos++;
                }
            } else {
                printf("Unknown character at %d:%d - '%c'.n\nAborting\n", this->line, this->col, c);
                std::exit(1);
            }
        }
    }
    this->tokens.push_back(Token(this->type, this->value, this->line, this->col));
}

std::vector<Token> Lexer::lex() {
    while (this->pos < this->data.length()) {
        c = this->data[this->pos];
        peek();
        this->pos++;
    }
    return this->tokens;
}