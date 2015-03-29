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
        if (cl == 0)
            continue;
        cl->Inherit(programScope);
    }

    Emit();

}

void Program::Emit() {
    codegen = new CodeGenerator;
    scoper = new Scoper(gpRelative, UP);
    for(int i = 0; i < decls->NumElements(); ++i) {
        VarDecl* varDecl = dynamic_cast<VarDecl*>(decls->Nth(i));
        if (varDecl != 0)
            varDecl->Emit(scoper, codegen, programScope);
    }
    // TODO incomplete function

}

StmtBlock::StmtBlock(List<VarDecl*>* d, List<Stmt*>* s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

bool StmtBlock::BuildTree(SymbolTable* symT) {

    blockScope = symT->addScope();
    bool flag = true;

    for (int i = 0; i < decls->NumElements(); ++i)
        flag = decls->Nth(i)->BuildTree(blockScope) ? flag : false;

    for (int i = 0; i < stmts->NumElements(); ++i)
        flag = stmts->Nth(i)->BuildTree(blockScope) ? flag : false;

    return flag;
}

void StmtBlock::Emit(Scoper *scopee, CodeGenerator *codegen, SymTable *symT) {
	for (int i = 0; i < decls->NumElements(); ++i)
		decls->Nth(i)->Emit(scopee, codegen, symT);
	for (int i = 0; i< stmts->NumElements(); ++i)
		stmts->Nth(i)->Emit(scopee, codegen, symT);
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

void ForStmt::Emit(Scoper *scopee, CodeGenerator *codegen, SymTable *symT) {
	char* endPoint = codegen->NewLabel();
	char* loopThing = codegen->NewLabel();
	init->Emit(scopee, codegen, blockScope);
	codegen->GenLabel(loopThing);
	test->Emit(scopee, codegen, blockScope);
	codegen->GenIfZ(test->get->gerFrameLoc(), endPoint);
	body->Emit(scopee, codegen, blockScope);
	step->Emit(scopee, codegen, blockScope);
	codegen->GenGoto(loopThing);
	codegen->GenLabel(endPoint);
}

bool WhileStmt::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();

    blockScope->setLastNode(this);
    return body->BuildTree(blockScope);
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


ReturnStmt::ReturnStmt(yyltype loc, Expr* e) : Stmt(loc) {
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}


PrintStmt::PrintStmt(List<Expr*>* a) {
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

bool PrintStmt::isPrintable(Type *type) {
  return (type->isConvertableTo(Type::intType) || type->isConvertableTo(Type::boolType) || type->isConvertableTo(Type::stringType));
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
    for (int i = 0; i < stmts->NumElements(); ++i)
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
    for (int i = 0; i < cases->NumElements(); ++i)
        flag = cases->Nth(i)->BuildTree(symT) ? flag : false;

    return flag;
}
