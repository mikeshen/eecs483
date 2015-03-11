/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"


Decl::Decl(Identifier* n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this);
}

VarDecl::VarDecl(Identifier* n, Type* t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}

bool VarDecl::BuildTree(SymbolTable* symT) {
    Symbol* sym = symT->findLocal(id->getName());
    if (sym != nullptr) {
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));
        return false;
    }
    symT->add(id->getName(), this);
    return true;
}

bool VarDecl::Check(SymbolTable* symT) {
    bool flag = type->Check(symT);

    if (!flag) {
        // TODO fix the get function reference here
        ReportError::IdentifierNotDeclared(nullptr, LookingForType);
        type = Type::errorType;
    }
    return flag;
}

ClassDecl::ClassDecl(Identifier* n, NamedType* ex, List<NamedType*>* imp, List<Decl*>* m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);
    extends = ex;
    if (extends)
		extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}

bool ClassDecl::BuildTree(SymbolTable* symT) {
    bool flag = true;
    Symbol* sym = symT->findLocal(id->getName());

    if (sym != nullptr) {
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));
        return false;
    }

    classScope = symT->addUnderScope(id->getName(), this, CLASS);
    classScope->setThis(classScope);

    for (int i = 0; i < members->NumElements(); ++i)
        flag = members->Nth(i)->BuildTree(classScope) ? flag : false;

    return true;
}

bool ClassDecl::Check(SymbolTable* symT) {
    return true;
/*    Assert(env != null && class_env != NULL);
    bool flag = CheckClassParents(env);
    flag = CheckClassInterfaces(env) ? flag : false;
    for (int i = 0; i < members->NumElements(); i++) {
        flag = members->Nth(i)->Check(env) ? flag : false;
    }
    return flag;
*/}


InterfaceDecl::InterfaceDecl(Identifier* n, List<Decl*>* m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
}


bool InterfaceDecl::BuildTree(SymbolTable* symT) {
    bool flag = true;
    Symbol* sym = symT->findLocal(id->getName());

    if (sym != nullptr) {
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));
        flag = false;
    }

    interfaceScope = symT->addUnderScope(id->getName(), this, INTERFACE);

    for (int i = 0; i < members->NumElements(); ++i)
        flag = members->Nth(i)->BuildTree(interfaceScope) ? flag : false;

    return flag;
}

bool InterfaceDecl::Check(SymbolTable* symT) {
    bool flag = true;
    for (int i = 0; i < members->NumElements(); i++)
        flag = members->Nth(i)->Check(symT) ? flag : false;
    return flag;
}


FnDecl::FnDecl(Identifier* n, Type* r, List<VarDecl*>* d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

bool FnDecl::BuildTree(SymbolTable* symT) {
    bool flag = true;
    Symbol* sym = symT->findLocal(id->getName());

    if (sym != nullptr) {
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));
        flag = false;
    }

    fnScope = symT->addUnderScope(id->getName(), this, FUNCTION);

    for (int i = 0; i < formals->NumElements(); ++i)
        flag = formals->Nth(i)->BuildTree(fnScope) ? flag : false;

    if (!(body->BuildTree(fnScope)))
        return false;
    return flag;
}

bool FnDecl::Check(SymbolTable* symT) {
    bool flag = returnType->Check(symT);
    for (int i = 0; i < formals->NumElements(); i++)
        flag = formals->Nth(i)->Check(symT) ? flag : false;

    if (body)
        flag = body->Check(symT) ? flag : false;
    return flag;
}
void FnDecl::SetFunctionBody(Stmt* b) {
    (body=b)->SetParent(this);
}
