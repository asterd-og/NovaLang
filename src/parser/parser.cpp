#include "parser.hpp"
#include <stdio.h>

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
    this->pos = 0;
}

void Parser::unexpectedError() {
    printf("Unexpected token '%s' at %d:%d.\nAborting\n",
            tok2str[this->get().type],
            this->get().line,
            this->get().col);
}

Token Parser::eat(TokType type) {
    if (this->tokens[this->pos].type != type) {
        printf("Expected %s at %d:%d but got %s.\nAborting\n",
               tok2str[type],
               this->tokens[this->pos].line,
               this->tokens[this->pos].col,
               tok2str[this->tokens[this->pos].type]);
        std::exit(1);
    }
    this->pos++;
    return this->tokens[this->pos - 1];
}

Token Parser::get() {
    return this->tokens[this->pos];
}

Token Parser::consume() {
    this->pos++;
    return this->tokens[this->pos - 1];
}

Node Parser::parseVarDecl() {
    this->right = this->consume();
    this->eat(TokType::semi);
    return Node(this->type, this->left, this->mid, this->right, this->innerNodes);
}

Node Parser::parseFunCall() {
    this->innerNodes.clear();
    this->eat(TokType::LPar);
    while (this->get().type != TokType::RPar) {
        this->innerNodes.push_back(Node(NodeType::callArg, this->consume()));
    }
    this->eat(TokType::RPar);
    this->eat(TokType::semi);
    return Node(this->type, this->left, this->mid, this->right, this->innerNodes);
}

std::vector<Node> Parser::parseBody() {
    this->eat(TokType::LBrac);

    std::vector<Node> ret;
    std::vector<Token> itoks;
    int opened = 1;

    while (opened > 0) {
        if (this->get().type == TokType::LBrac) opened++;
        else if (this->get().type == TokType::RBrac) {
            opened--;
            if (opened == 0) break;
        }
        itoks.push_back(this->consume());
    }

    Parser p(itoks);
    p.parse();

    this->eat(TokType::RBrac);
    ret = p.ast;

    return ret;
}

Node Parser::parseFunDecl() {
    this->innerNodes.clear();
    this->eat(TokType::RPar);
    this->innerNodes = this->parseBody();
    return Node(this->type, this->left, this->mid, this->right, this->innerNodes);
}

Node Parser::parseDecl() {
    this->consume();
    this->mid = this->eat(TokType::name);
    switch (this->get().type) {
        case TokType::eq:
            this->type = NodeType::varDecl;
            this->consume();
            return this->parseVarDecl();
            break;
        case TokType::LPar:
            this->type = NodeType::funDecl;
            this->consume();
            return this->parseFunDecl();
            break;
        default:
            this->unexpectedError();
            std::exit(1);
            break;

    }
}

Node Parser::parseStmt() {
    this->consume();
    switch (this->get().type) {
        case TokType::LPar:
            this->type = NodeType::funCall;
            return this->parseFunCall();
            break;
        default:
            this->unexpectedError();
            std::exit(1);
            break;
    }
    
}

Node Parser::parseRet() {
    this->type = ret;
    this->left = this->consume();
    this->eat(TokType::semi); // For now we just consume 1 token
    // Later on we will eat everything until a semi
    return Node(this->type, this->left, this->mid, this->right, this->innerNodes);
}

Node Parser::parseExpr() {
    switch (this->get().type) {
        case TokType::type:
            this->left = get();
            return this->parseDecl();
        case TokType::name:
            this->left = get();
            return this->parseStmt();
        case TokType::retkyd:
            this->consume();
            return this->parseRet();
        default:
            this->unexpectedError();
            std::exit(1);
            break;
    }
}

void Parser::parse() {
    while (this->pos < this->tokens.size()) {
        this->ast.push_back(this->parseExpr());
    }
}