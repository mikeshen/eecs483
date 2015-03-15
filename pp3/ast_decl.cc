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
    if (sym != NULL) {
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

    if (sym != NULL) {
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));
        return false;
    }

    classScope = symT->addUnderScope(id->getName(), this, CLASS);
    classScope->setThis(classScope);

    for (int i = 0; i < members->NumElements(); ++i)
        flag = members->Nth(i)->BuildTree(classScope) ? flag : false;

    return true;
}

bool ClassDecl::FulfillsInterface(char *name) {
  for (int i = 0; i < implements->NumElements(); ++i)
    if (strcmp(name, implements->Nth(i)->getName()) == 0)
      return true;
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ClassDecl::CheckClassParents(SymbolTable *symT) {
    bool flag = true;
    Symbol *s;
    if (extends && (s = symT->find(extends->getName())) == NULL) {
        flag = false;
        ReportError::IdentifierNotDeclared(extends->getIdentifier(), LookingForClass);
    }

    for (int i = 0; i < members->NumElements(); ++i) {
        VarDecl *field;
        FnDecl *method = dynamic_cast<FnDecl*>(members->Nth(i));
        if (method != 0) {
            if ((s = classScope->findSuper(method->getName(), FUNCTION)) != NULL) {
                FnDecl *otherMethod = dynamic_cast<FnDecl*>(s->getNode());
                if (!method->isValidFn(otherMethod)) {
                    flag = false;
                    ReportError::OverrideMismatch(method);
                }
            }
        }
        else {
            field = dynamic_cast<VarDecl*>(members->Nth(i));
            if ((s = classScope->findSuper(field->getName(), VARIABLE)) != NULL) {
                flag = false;
                ReportError::DeclConflict(field, dynamic_cast<Decl*>(s->getNode()));
            }
        }
    }
    return flag;
}

bool ClassDecl::CheckClassInterfaces(SymbolTable *symT) {
    bool flag = true;
    ImplementedFunction *vf = NULL;
    Iterator<ImplementedFunction*> iter = implmentedFunctions->GetIterator();
    Hashtable<NamedType*> *incompleteInterFaces = new Hashtable<NamedType*>;
    for (int i = 0; i < implements->NumElements(); ++i) {
        NamedType *intf = implements->Nth(i);
        Symbol* sym = symT->find(intf->getName());
        if (sym == NULL) {
            ReportError::IdentifierNotDeclared(intf->getIdentifier(), LookingForInterface);
            flag = false;
        }
    }
    while ((vf = iter.GetNextValue()) != NULL) {
         Symbol* sym = classScope->findLocal(vf->getPrototype()->getName(), FUNCTION);
        if (sym == NULL) {
            incompleteInterFaces->Enter(vf->getIntfType()->getName(), vf->getIntfType(), false);
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
    ReportBadInterfaces(incompleteInterFaces);
    delete incompleteInterFaces;
    return flag;
}

void ClassDecl::ReportBadInterfaces(Hashtable<NamedType*>* incompleteInterFaces) {
    Iterator<NamedType*> iter2 = incompleteInterFaces->GetIterator();
    NamedType *intf_type = NULL;
    while ((intf_type = iter2.GetNextValue()) != NULL)
        ReportError::InterfaceNotImplemented(this, intf_type);
}

bool ClassDecl::Check(SymbolTable* symT) {
    bool flag = true;
    flag = CheckClassParents(symT) ? flag : false;
    flag = CheckClassInterfaces(symT) ? flag : false;
    for (int i = 0; i < members->NumElements(); ++i)
        flag = members->Nth(i)->Check(symT) ? flag : false;
    return flag;
}

bool ClassDecl::Inherit(SymbolTable *symT) {
    bool flag = true;
    Symbol* base;
    if (extends && (base = symT->find(extends->getName(), CLASS)) != NULL) {
        classScope->setSuper(base->getEnv());
        parent = dynamic_cast<ClassDecl*>(base->getNode());
        Assert(parent != 0);
    }

    implmentedFunctions = new Hashtable<ImplementedFunction*>;
    for (int i = 0; i < implements->NumElements(); ++i) {
        NamedType* interface = implements->Nth(i);
        Symbol* sym = NULL;
        if ((sym = symT->find(interface->getName(), INTERFACE)) == NULL)
            continue;
        InterfaceDecl* dec = dynamic_cast<InterfaceDecl*>(sym->getNode());
        Assert(dec != 0);
        List<Decl*>* m = dec->getMembers();
        for (int j = 0; j < m->NumElements(); ++j) {
            FnDecl* fn = dynamic_cast<FnDecl*>(m->Nth(j));
            Assert(fn != 0);
            ImplementedFunction* vf = NULL;
            if ((vf = implmentedFunctions->Lookup(fn->getName())) == NULL) {
                implmentedFunctions->Enter(fn->getName(), new ImplementedFunction(fn, implements->Nth(i)));
                continue;
            }

            if (!fn->isValidFn(vf->getPrototype())) {
                flag = false;
                ReportError::OverrideMismatch(fn);
            }
        }
    }
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

    if (sym != NULL) {
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

    if (sym != NULL) {
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));
        flag = false;
    }

    fnScope = symT->addUnderScope(id->getName(), this, FUNCTION);

    for (int i = 0; i < formals->NumElements(); ++i)
        flag = formals->Nth(i)->BuildTree(fnScope) ? flag : false;

    if (body && !(body->BuildTree(fnScope)))
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
