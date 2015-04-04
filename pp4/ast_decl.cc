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

void VarDecl::Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable *symT) {
	Symbol* s = symT->find(id->getName(), VARIABLE);
	Location* l = scopee->Alloc(id->getName(), 4);
	s->setLoc(l);
}

ClassDecl::ClassDecl(Identifier* n, NamedType* ex, List<NamedType*>* imp, List<Decl*>* m) : Decl(n),
    classScope(NULL), implmentedFunctions(NULL), parent(NULL), classScoper(NULL),
    vTable(NULL), emittedMethods(NULL), fields(NULL), numFields(0) {
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

    if (sym != NULL)
        ReportError::DeclConflict(this, static_cast<Decl*>(sym->getNode()));

    classScope = symT->addUnderScope(id->getName(), this, CLASS);
    classScope->setThis(classScope);

    for (int i = 0; i < members->NumElements(); ++i)
        flag = members->Nth(i)->BuildTree(classScope) ? flag : false;

    return flag;
}

bool ClassDecl::FulfillsInterface(char *name) {
  for (int i = 0; i < implements->NumElements(); ++i)
    if (strcmp(name, implements->Nth(i)->getName()) == 0)
      return true;
  return false;
}

void ClassDecl::EmitHelper(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    if (classScoper != NULL) return;

    classLabel = codegen->NewLabel();
    vTable = new Hashtable<FnDecl*>;
    fields = new List<VarDecl*>;

    // recursve emit
    if (!extends)
        classScoper = new Scoper(cRelative, UP);
    else {
        parent->EmitHelper(scopee, codegen, symT);

        List<VarDecl*>* parentFields = parent->getFields();
        for (int i = 0; i < parentFields->NumElements(); ++i)
            fields->Append(parentFields->Nth(i));
        auto i = (parent->getVTable())->GetIterator();
        FnDecl* fnDecl;
        while ((fnDecl = i.GetNextValue()) != NULL)
            vTable->Enter(fnDecl->getName(), fnDecl);
        classScoper = new Scoper(parent->getScoper());
    }

    emittedMethods = new List<FnDecl*>;
    for (int i = 0; i < members->NumElements(); ++i) {
        FnDecl* tempMethod = dynamic_cast<FnDecl*>(members->Nth(i));
        if (tempMethod == 0) { // is field
            VarDecl* tempField = dynamic_cast<VarDecl*>(members->Nth(i));
            fields->Append(tempField);
            tempField->Emit(classScoper, codegen, symT);
        }
        else {
            vTable->Enter(tempMethod->getName(), tempMethod);
            tempMethod->setMethodLabel(classLabel);
            emittedMethods->Append(tempMethod);
        }
    }
    auto i = vTable->GetIterator();
    int count = 0;
    FnDecl* tempMethod;
    while ((tempMethod = i.GetNextValue()) != NULL)
        tempMethod->setOffset(count++);
}

void ClassDecl::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    for (int i = 0; i < emittedMethods->NumElements(); ++i)
        emittedMethods->Nth(i)->EmitMethod(this, classScoper, codegen, symT);

    numFields = fields->NumElements();
    List<const char*>* methodLabels = new List<const char*>;
    auto i = vTable->GetIterator();
    FnDecl* fnDecl;
    while ((fnDecl = i.GetNextValue()) != NULL)
        methodLabels->Append(fnDecl->getMethodLabel());
    codegen->GenVTable(classLabel, methodLabels);
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

        }
    }
  return flag;
}

InterfaceDecl::InterfaceDecl(Identifier* n, List<Decl*>* m) : Decl(n), interfaceScope(NULL) {
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

FnDecl::FnDecl(Identifier* n, Type* r, List<VarDecl*>* d) : Decl(n), fnScope(NULL),
               paramScoper(NULL), bodyScoper(NULL), methodLabel(NULL),
               functionLabel(NULL), offset(0) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::SetFunctionBody(Stmt* b) {
    (body=b)->SetParent(this);
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

void FnDecl::EmitMethod(ClassDecl* classDecl, Scoper* scoper,
                        CodeGenerator* codegen, SymbolTable* symT) {
    BeginFunc* beginFn;
    paramScoper = new Scoper(fpRelative, UP);
    bodyScoper = new Scoper(fpRelative, DOWN);
    codegen->GenLabel(methodLabel);

    beginFn = codegen->GenBeginFunc();
    char* fnThis = strdup("this");
    Location* paramThis = paramScoper->Alloc(fnThis, 4);
    fnScope->add(fnThis, NULL, paramThis);
    for (int i = 0; i < formals->NumElements(); ++i)
        formals->Nth(i)->Emit(paramScoper, codegen, fnScope);
    body->Emit(bodyScoper, codegen, fnScope);
    beginFn->SetFrameSize(bodyScoper->GetSize());
    codegen->GenEndFunc();
}

void FnDecl::Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable *symT) {
    BeginFunc *beginFn;
    paramScoper = new Scoper(fpRelative, UP);
    bodyScoper = new Scoper(fpRelative, DOWN);

    functionLabel = codegen->NewLabel();
    codegen->GenLabel(functionLabel);
    beginFn = codegen->GenBeginFunc();
    for (int i = 0; i < formals->NumElements(); ++i)
        formals->Nth(i)->Emit(paramScoper, codegen, fnScope);
    body->Emit(bodyScoper, codegen, fnScope);
    beginFn->SetFrameSize(bodyScoper->GetSize());
    codegen->GenEndFunc();
}

void FnDecl::setMethodLabel(char *label) {
    int len = strlen(label) + strlen(getName()) + 2;
    methodLabel = (char*) malloc(len);
    sprintf(methodLabel, "%s.%s", label, getName());
    methodLabel[len - 1] = '\0';
}
