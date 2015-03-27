/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>
#include <sstream>

/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

Type* Type::intType    = new Type("int");
Type* Type::doubleType = new Type("double");
Type* Type::voidType   = new Type("void");
Type* Type::boolType   = new Type("bool");
Type* Type::nullType   = new Type("null");
Type* Type::stringType = new Type("string");
Type* Type::errorType  = new Type("error");

extern SymbolTable* globalEnv;

Type::Type(const char* n) {
    Assert(n);
    typeName = strdup(n);
}

bool NamedType::isConvertableTo(Type* inputType) {
    Symbol* sym = globalEnv->find(id->getName(), CLASS);
    ClassDecl* classDecl = NULL;
    char* newName = inputType->getName();

    if (inputType->isBuiltIn())
        return false;
    else if (dynamic_cast<NamedType*>(inputType) != 0 && strcmp(id->getName(), dynamic_cast<NamedType*>(inputType)->getName()) == 0)
        return true;
    else if (sym == NULL)
        return false;
    classDecl = dynamic_cast<ClassDecl*>(sym->getNode());
    if (classDecl->FulfillsInterface(newName))
        return true;
    else
        return sym->getEnv()->subclassOf(newName);
}

bool NamedType::isEquivalentTo(Type* inputType) {
    NamedType* otherType = dynamic_cast<NamedType*>(inputType);
    if (otherType == 0)
        return false;
    return strcmp(id->getName(), otherType->getName()) == 0;
}


NamedType::NamedType(Identifier* i) : Type(*i->GetLocation()) {
    Assert(i != NULL);
    (id=i)->SetParent(this);
}

ArrayType::ArrayType(yyltype loc, Type* et) : Type(loc) {
    Assert(et != NULL);
    (elemType=et)->SetParent(this);
}


bool ArrayType::isConvertableTo(Type* inputType) {
    std::ostringstream os1;
    std::ostringstream os2;
    PrintToStream(os1);
    inputType->PrintToStream(os2);
    return os1.str() == os2.str();
}


bool ArrayType::isEquivalentTo(Type* inputType) {
    std::ostringstream os1;
    std::ostringstream os2;
    PrintToStream(os1);
    inputType->PrintToStream(os2);
    return os1.str() == os2.str();
}