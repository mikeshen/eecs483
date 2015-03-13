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
        ReportError::IdentifierNotDeclared(type->getIdentifier(), LookingForType);
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

bool ClassDecl::ImplementsInterface(char *name) {
  for (int i = 0; i < implements->NumElements(); ++i)
    if (strcmp(name, implements->Nth(i)->getName()) == 0)
      return true;
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ClassDecl::CheckAgainstParents(SymbolTable *symT) {
    bool flag = true;
    Symbol *s = NULL;
    // Check that parent exists
    if (extends && (s = symT->find(extends->getName())) != NULL) {
        ReportError::IdentifierNotDeclared(extends->getIdentifier(), LookingForClass);
        flag = false;
    }

    for (int i = 0; i < members->NumElements(); ++i) {
        FnDecl *method = dynamic_cast<FnDecl*>(members->Nth(i));
        VarDecl *field = NULL;
        if (method != 0) {
            if ((s = classScope->findSuper(method->getName(), FUNCTION)) != NULL) {
                FnDecl *otherMethod = dynamic_cast<FnDecl*>(s->getNode());
                if (!method->isValidFn(otherMethod)) {
                    ReportError::OverrideMismatch(method);
                    flag = false;
                }
            }
        }
        else {
            field = dynamic_cast<VarDecl*>(members->Nth(i));
            if ((s = classScope->findSuper(field->getName(), VARIABLE)) != NULL) {
                ReportError::DeclConflict(field, dynamic_cast<Decl*>(s->getNode()));
                flag = false;
            }
        }
    }
    return flag;
}

bool ClassDecl::CheckAgainstInterfaces(SymbolTable *symT) {
    bool flag = true;
    FnCheckles *vf = NULL;
    Iterator<FnCheckles*> iter = funcCheckles->GetIterator();
    Hashtable<NamedType*> *incompleteIntfs = new Hashtable<NamedType*>;
    // Check that all interfaces implemented exists
    for (int i = 0; i < implements->NumElements(); ++i) {
        NamedType *intf = implements->Nth(i);
        Symbol* sym = symT->find(intf->getName());
        if (sym == NULL) {
            ReportError::IdentifierNotDeclared(intf->getIdentifier(), LookingForInterface);
            flag = false;
        }
    }
    // Check each interface's methods have been implemented with correct type
    // signature. Otherwise, give OverrideMismatch error
    while ((vf = iter.GetNextValue()) != NULL) {
         Symbol* sym = classScope->findInClass(vf->getPrototype()->getName(), FUNCTION);
        if (sym == NULL) {
            incompleteIntfs->Enter(vf->getIntfType()->getName(), vf->getIntfType(), false);
            flag = false;
            continue;
        }
        FnDecl* method = dynamic_cast<FnDecl*>(sym->getNode());
        Assert(method != 0);
        if (!method->isValidFn(vf->getPrototype())) {
            ReportError::OverrideMismatch(method);
            flag = false;
        }
    }
    // Report interfaces that have not been implemented
    Iterator<NamedType*> iter2 = incompleteIntfs->GetIterator();
    NamedType *intf_type = NULL;
    while ((intf_type = iter2.GetNextValue()) != NULL) {
        ReportError::InterfaceNotImplemented(this, intf_type);
    }
    delete incompleteIntfs;
    return flag;
}

bool ClassDecl::Check(SymbolTable* symT) {
    bool flag = true;
    flag = CheckAgainstParents(symT) ? flag : false;
    //flag = CheckAgainstInterfaces(symT) ? flag : false;
    for (int i = 0; i < members->NumElements(); ++i)
        flag = members->Nth(i)->Check(symT) ? flag : false;
    return flag;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

bool FnDecl::isValidFn(FnDecl* fn) {
    if (!returnType->isEquivalentTo(fn->GetReturnType()))
        return false;
    List<VarDecl*>* formalsCheck = fn->GetFormals();
    if (formals->NumElements() != formalsCheck->NumElements())
        return false;
    for (int i = 0; i < formalsCheck->NumElements(); i++)
        if (!formalsCheck->Nth(i)->getType()->isEquivalentTo(formals->Nth(i)->getType()))
            return false;
    return true;
}
