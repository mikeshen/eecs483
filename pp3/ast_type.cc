/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>

/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error");

extern SymTable *globalEnv;

Type::Type(const char *n) {
    Assert(n);
    typeName = strdup(n);
}

<<<<<<< HEAD
=======
bool NamedType::IsConvertableTo(Type *theType) {
  Symbol *sym = globalEnv->find(id->getName(), CLASS);
  ClassDecl *classDecl = static_cast<ClassDecl*>(sym->getNode());
  char *newName = theType->getName();

  if (theType->IsBuiltin())
    return false;

  if (IsEquivalentTo(theType))
    return true;

  if (sym  == nullptr)
    return false;

  if (classDecl->ImplementsInterface(newName))
    return true;

  return sym->getEnv()->subclassOf(newName);
}

bool NamedType::IsEquivalentTo(Type *theType) {
  NamedType *ntheType = dynamic_cast<NamedType*>(theType);
  if (ntheType == 0)
    return false;
  return strcmp(id->name(), ntheType->getName()) == 0;
}

	
>>>>>>> origin/master
NamedType::NamedType(Identifier *i) : Type(*i->GetLocation()) {
    Assert(i != NULL);
    (id=i)->SetParent(this);
}

ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
    Assert(et != NULL);
    (elemType=et)->SetParent(this);
}

bool ArrayType::IsConvertableTo(Type *theType) {
  if (theType->IsEquivalentTo(Type::errorType))
    return true;

  ArrayType *ntheType = dynamic_cast<ArrayType*>(theType);
  if (ntheType == 0)
    return false;

  return elemType->IsEquivalentTo(ntheType->getElemType());
}

bool ArrayType::IsEquivalentTo(Type *theType) {
  ArrayType *ntheType = dynamic_cast<ArrayType*>(theType);
  if (ntheType == 0) {
    return false;
  }
  return elemType->IsEquivalentTo(ntheType->getElemType());
}

