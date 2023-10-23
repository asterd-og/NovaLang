#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "../lexer/token.hpp"

typedef enum {
    varDecl,
    funDecl,
    funCall,
    funArgs,
    callArg,
    ret
} NodeType;

static const char* node2str[] = {
    "varDecl",
    "funDecl",
    "funCall"
};

class Node {
    public:
    NodeType type;
    Token left;
    Token mid;
    Token right;
    std::vector<Node> innerNodes;

    inline Node(NodeType type, Token left, Token mid, Token right, std::vector<Node> innerNodes) {
        this->type = type;
        this->left = left;
        this->mid = mid;
        this->right = right;
        this->innerNodes = innerNodes;
    }

    inline Node(NodeType type, Token left, Token mid, Token right) {
        this->type = type;
        this->left = left;
        this->mid = mid;
        this->right = right;
    }

    inline Node(NodeType type, Token left, Token mid) {
        this->type = type;
        this->left = left;
        this->mid = mid;
    }

    inline Node(NodeType type, Token left) {
        this->type = type;
        this->left = left;
    }

    inline Node() {
    }
};