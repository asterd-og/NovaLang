#include "tp.hpp"
#include <stdio.h>
#include <sstream>
#include <stdarg.h>

std::map<std::string, std::string> nTypeToC = {

    {"u8", "unsigned char"},
    {"u16", "unsigned short"},
    {"u32", "unsigned int"},
    {"u64", "unsigned long long"},

    {"i8", "signed char"},
    {"i16", "signed short"},
    {"i32", "signed int"},
    {"i64", "signed long long"},

    {"v0", "void"},
    
    {"int", "int"},
    {"str", "char*"}
};

std::string stringf(char* str, ...) {
    va_list args;
    va_start(args, str);

    std::ostringstream ss;

    char ch = 0;
    while (ch = *str++) {
        if (ch == '%') {
            switch (ch = *str++) {
                case 's':
                    ss << va_arg(args, char*);
                    break;
                case 'd':
                    ss << va_arg(args, int);
                    break;
            }
        } else {
            ss << ch;
        }
    }

    va_end(args);

    return ss.str();
}

void Tp::evalVarDecl() {
    if (this->insideScope) {
        this->sVars[this->scope].push_back(this->node.mid.value);
    } else {
        this->gVars.push_back(this->node.mid.value);
    }
    this->code += stringf("%s %s=%s;\n", nTypeToC[this->node.left.value].c_str(),
        this->node.mid.value.c_str(),
        this->node.right.value.c_str());
}

void Tp::evalFunDecl() {
    if (this->insideScope) {
        exit(1);
    }
    this->insideScope = true;
    this->scope = this->node.mid.value;

    Tp t;

    this->code += stringf("%s %s() {\n%s}\n", nTypeToC[this->node.left.value].c_str(),
        this->node.mid.value.c_str(),
        t.transpile(this->node.innerNodes).c_str());
}

Token Tp::getArg(int pos) {
    return this->node.innerNodes[pos].left;
}

void Tp::evalFunCall() {
    std::ostringstream ss;
    for (auto inode : this->node.innerNodes) {
        ss << inode.left.value.c_str(); // for now just append everything
    }
    this->code += stringf("%s(%s);\n", this->node.left.value.c_str(), ss.str().c_str());
}

void Tp::evalRet() {
    this->code += stringf("return %s;\n", this->node.left.value.c_str());
}

std::string Tp::transpile(std::vector<Node> ast) {
    this->ast = ast;
    this->insideScope = false;
    this->code = "";
    
    for (Node node : ast) {
        this->node = node;
        switch (node.type) {
            case NodeType::varDecl:
                this->evalVarDecl();
                break;
            case NodeType::funCall:
                this->evalFunCall();
                break;
            case NodeType::funDecl:
                this->evalFunDecl();
                break;
            case NodeType::ret:
                this->evalRet();
                break;
        }
    }

    return this->code.c_str();
}