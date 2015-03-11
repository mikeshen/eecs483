/* File: ast.cc
 * ------------
 */

#include "ast.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h> // strdup
#include <stdio.h>  // printf

Node::Node(yyltype loc) {
    location = new yyltype(loc);
    parent = NULL;
}

Node::Node() {
    location = NULL;
    parent = NULL;
}

Identifier::Identifier(yyltype loc, const char *n) : Node(loc) {
    name = strdup(n);
}

bool Identifier::Check(SymbolTable* symT) {
    return symT->find(name) != nullptr;
}

bool Identifier::Check(SymbolTable* symT, E_Type type) {
    return symT->find(name, type) != nullptr;
}
