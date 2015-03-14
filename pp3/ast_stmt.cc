/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"

SymbolTable* globalEnv;

Program::Program(List<Decl*>* d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::Check() {
    /* pp3: here is where the semantic analyzer is kicked off.
    *       The general idea is perform a tree traversal of the
    *       entire program, examining all constructs for compliance
    *       with the semantic rules.  Each node can have its own way of
    *       checking itself, which makes for a great use of inheritance
    *       and polymorphism in the node classes.
    */

    ClassDecl* cl;
    programScope = new SymbolTable();
    globalEnv = programScope;

    for (int i = 0; i < decls->NumElements(); i++)
        decls->Nth(i)->BuildTree(programScope);

    for (int i = 0; i < decls->NumElements(); i++) {
        cl = dynamic_cast<ClassDecl*>(decls->Nth(i));
        if (cl == 0) {
            continue;
        }
        cl->Inherit(programScope);
    }

    for (int i = 0; i < decls->NumElements(); i++)
        decls->Nth(i)->Check(programScope);
}

StmtBlock::StmtBlock(List<VarDecl*>* d, List<Stmt*>* s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

bool StmtBlock::BuildTree(SymbolTable* symT) {

    blockScope = symT->addScope();
    bool flag = true;

    for (int i = 0; i < decls->NumElements(); i++)
        flag = decls->Nth(i)->BuildTree(blockScope) ? flag : false;

    for (int i = 0; i < stmts->NumElements(); i++)
        flag = stmts->Nth(i)->BuildTree(blockScope) ? flag : false;

    return flag;
}

bool StmtBlock::Check(SymbolTable* symT) {
    bool flag = true;

    for (int i = 0; i < decls->NumElements(); i++) {
        flag = decls->Nth(i)->Check(blockScope) ? flag : false;
    }

    for (int i = 0; i < stmts->NumElements(); i++) {
        // flag = stmts->Nth(i)->Check(blockScope) ? flag : false;
        Stmt* st = stmts->Nth(i);
        st->Check(blockScope);
    }
    return flag;
}

ConditionalStmt::ConditionalStmt(Expr* t, Stmt* b) {
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this);
    (body=b)->SetParent(this);
}

ForStmt::ForStmt(Expr* i, Expr* t, Expr* s, Stmt* b): LoopStmt(t, b) {
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

bool ForStmt::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();
    blockScope->setLastNode(this);
    return body->BuildTree(blockScope);
}

bool ForStmt::Check(SymbolTable* symT) {
    bool flag = test->Check(symT);

    if (!test->getEvalType()->isConvertableTo(Type::boolType)) {
        ReportError::TestNotBoolean(test);
        flag = false;
    }

    flag = init->Check(symT) ? flag : false;
    flag = step->Check(symT) ? flag : false;
    flag = body->Check(blockScope) ? flag : false;
    return flag;
}

bool WhileStmt::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();

    blockScope->setLastNode(this);
    return body->BuildTree(blockScope);
}

bool WhileStmt::Check(SymbolTable* symT) {
    bool flag = test->Check(symT);
    if (!test->getEvalType()->isConvertableTo(Type::boolType)) {
      ReportError::TestNotBoolean(test);
      flag = false;
    }
    flag = body->Check(blockScope) ? flag : false;
    return flag;
}

IfStmt::IfStmt(Expr* t, Stmt* tb, Stmt* eb): ConditionalStmt(t, tb) {
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

bool IfStmt::BuildTree(SymbolTable* symT) {
    bool flag = true;
    flag = body->BuildTree(symT) ? flag : false;
    if (elseBody != NULL)
        flag = elseBody->BuildTree(symT) ? flag : false;
    return flag;
}

bool IfStmt::Check(SymbolTable* symT) {
    bool flag = test->Check(symT);
    if (!test->getEvalType()->isConvertableTo(Type::boolType)) {
      ReportError::TestNotBoolean(test);
      flag = false;
    }
    flag = body->Check(symT) ? flag : false;
    if (elseBody)
      flag = elseBody->Check(symT) ? flag : false;
    return flag;
}

bool BreakStmt::Check(SymbolTable* symT) {
    if (symT->getLastNode() == NULL) {
      ReportError::BreakOutsideLoop(this);
      return false;
    }
    return true;
}

ReturnStmt::ReturnStmt(yyltype loc, Expr* e) : Stmt(loc) {
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

bool ReturnStmt::Check(SymbolTable* symT) {
    bool flag = expr->Check(symT);
    FnDecl* fn = dynamic_cast<FnDecl*>(symT->getOwnerNode());
    if (!expr->getEvalType()->isConvertableTo(fn->GetReturnType())) {
        ReportError::ReturnMismatch(this, expr->getEvalType(), fn->GetReturnType());
        return false;
    }
    return flag;
}

PrintStmt::PrintStmt(List<Expr*>* a) {
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

bool PrintStmt::isPrintable(Type *type) {
  return (type->isConvertableTo(Type::intType) || type->isConvertableTo(Type::boolType) || type->isConvertableTo(Type::stringType));
}

bool PrintStmt::Check(SymbolTable* symT) {
    bool flag = true;
    for (int i = 0; i < args->NumElements(); i++) {
      flag = args->Nth(i)->Check(symT) ? flag : false;
      Type* argType = args->Nth(i)->getEvalType();
      if (!isPrintable(argType)) {
        ReportError::PrintArgMismatch(args->Nth(i), i+1, argType);
        flag = false;
      }
    }
    return flag;
}

Case::Case(IntConstant* v, List<Stmt*>* s) {
    Assert(s != NULL);
    value = v;
    if (value) value->SetParent(this);
    (stmts=s)->SetParentAll(this);
}

// deprecated
bool Case::BuildTree(SymbolTable* symT) {
    caseScope = symT->addScope();
    bool flag = true;

    caseScope->setLastNode(this);
    for (int i = 0; i < stmts->NumElements(); i++)
        flag = stmts->Nth(i)->BuildTree(symT) ? flag : false;

    return flag;
}

SwitchStmt::SwitchStmt(Expr* e, List<Case*>* c) {
    Assert(e != NULL && c != NULL);
    (expr=e)->SetParent(this);
    (cases=c)->SetParentAll(this);
}

// deprecated
bool SwitchStmt::BuildTree(SymbolTable* symT) {
    bool flag = true;
    for (int i = 0; i < cases->NumElements(); i++)
        flag = cases->Nth(i)->BuildTree(symT) ? flag : false;

    return flag;
}
