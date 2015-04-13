/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include <sstream>

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

    for (int i = 0; i < decls->NumElements(); i++) {
        ClassDecl* classDecl = dynamic_cast<ClassDecl*>(decls->Nth(i));
        if (classDecl != 0) 
            classDecl->EmitHelper(scoper, codegen, programScope);
    }

    for (int i = 0; i < decls->NumElements(); i++) {
        ClassDecl* classDecl = dynamic_cast<ClassDecl*>(decls->Nth(i));
        if (classDecl != 0)
            classDecl->Emit(scoper, codegen, programScope);
    }

    for (int i = 0; i < decls->NumElements(); i++) {
      FnDecl *fnDecl = dynamic_cast<FnDecl*>(decls->Nth(i));
      if (fnDecl != 0)
        fnDecl->Emit(scoper, codegen, programScope);
    }

    codegen->DoFinalCodeGen();

}

StmtBlock::StmtBlock(List<VarDecl*>* d, List<Stmt*>* s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();

    for (int i = 0; i < decls->NumElements(); ++i)
        decls->Nth(i)->BuildTree(blockScope);

    for (int i = 0; i < stmts->NumElements(); ++i)
        stmts->Nth(i)->BuildTree(blockScope);
}

void StmtBlock::Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT) {
	for (int i = 0; i < decls->NumElements(); ++i)
		decls->Nth(i)->Emit(scopee, codegen, blockScope);
	for (int i = 0; i< stmts->NumElements(); ++i)
		stmts->Nth(i)->Emit(scopee, codegen, blockScope);
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

void ForStmt::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();
    blockScope->setLastNode(this);
    body->BuildTree(blockScope);
}

void ForStmt::Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT) {
	nextLabel = codegen->NewLabel();
	char* loopThing = codegen->NewLabel();
	init->Emit(scopee, codegen, blockScope);
	codegen->GenLabel(loopThing);
	test->Emit(scopee, codegen, blockScope);
    Assert(test->getFramePosition());
	codegen->GenIfZ(test->getFramePosition(), nextLabel);
	body->Emit(scopee, codegen, blockScope);
	step->Emit(scopee, codegen, blockScope);
	codegen->GenGoto(loopThing);
	codegen->GenLabel(nextLabel);
}

void WhileStmt::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();

    blockScope->setLastNode(this);
    body->BuildTree(blockScope);
}

void WhileStmt::Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT) {
    nextLabel = codegen->NewLabel();
    char* loopThing = codegen->NewLabel();
    codegen->GenLabel(loopThing);
    test->Emit(scopee, codegen, blockScope);
    codegen->GenIfZ(test->getFramePosition(), nextLabel);
    body->Emit(scopee, codegen, blockScope);
    codegen->GenGoto(loopThing);
    codegen->GenLabel(nextLabel);
}

IfStmt::IfStmt(Expr* t, Stmt* tb, Stmt* eb): ConditionalStmt(t, tb) {
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::BuildTree(SymbolTable* symT) {
    blockScope = symT->addScope();
    body->BuildTree(blockScope);
    if (elseBody != NULL)
        elseBody->BuildTree(blockScope);
}

void IfStmt::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    char* conditionLabel = codegen->NewLabel();

    test->Emit(scopee, codegen, blockScope);
    codegen->GenIfZ(test->getFramePosition(), conditionLabel);
    body->Emit(scopee, codegen, blockScope);
    if(!elseBody) {
        codegen->GenLabel(conditionLabel);
        return;
    }

    char* elseBlock = codegen->NewLabel();
    codegen->GenGoto(elseBlock);
    codegen->GenLabel(conditionLabel);
    elseBody->Emit(scopee, codegen, blockScope);
    codegen->GenLabel(elseBlock);
}

void BreakStmt::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    LoopStmt* loopNode = dynamic_cast<LoopStmt*>(symT->getLastNode());
    codegen->GenGoto(loopNode->GetNextLabel());
}

ReturnStmt::ReturnStmt(yyltype loc, Expr* e) : Stmt(loc) {
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

void ReturnStmt::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    expr->Emit(scopee, codegen, symT);
    codegen->GenReturn(expr->getFramePosition());
}


Type* convertArray(Type* type) {
    ArrayType* array = dynamic_cast<ArrayType*>(type);
    if( array != 0 ) {
        std::ostringstream os1;
        array->PrintToStream(os1);
        string s = os1.str();
        string sub = s.substr(0, s.find("["));
        if(sub == "int")
            type = Type::intType;
        if(sub == "string")
            type = Type::stringType;
        if(sub == "bool")
            type = Type::boolType;
    }
    return type;
}

PrintStmt::PrintStmt(List<Expr*>* a) {
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

bool PrintStmt::isPrintable(Type *type) {
  return (type->isConvertableTo(Type::intType) || type->isConvertableTo(Type::boolType) || type->isConvertableTo(Type::stringType));
}

void PrintStmt::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    for (int i = 0; i < args->NumElements(); ++i) {
        Expr* arg = args->Nth(i);
        arg->Emit(scopee, codegen, symT);
        Location* loc = arg->getFramePosition();
        Type* type = arg->getEvalType(symT);


        if(type->isEquivalentTo(Type::intType))
            codegen->GenBuiltInCall(PrintInt, scopee, loc, NULL);
        else if(type->isEquivalentTo(Type::boolType))
            codegen->GenBuiltInCall(PrintBool, scopee, loc, NULL);
        else if(type->isEquivalentTo(Type::stringType))
            codegen->GenBuiltInCall(PrintString, scopee, loc, NULL);
    }
}
Case::Case(IntConstant* v, List<Stmt*>* s) {
    Assert(s != NULL);
    value = v;
    if (value) value->SetParent(this);
    (stmts=s)->SetParentAll(this);
}

// deprecated
void Case::BuildTree(SymbolTable* symT) {
    caseScope = symT->addScope();


    caseScope->setLastNode(this);
    for (int i = 0; i < stmts->NumElements(); ++i)
        stmts->Nth(i)->BuildTree(symT);
}

SwitchStmt::SwitchStmt(Expr* e, List<Case*>* c) {
    Assert(e != NULL && c != NULL);
    (expr=e)->SetParent(this);
    (cases=c)->SetParentAll(this);
}

// deprecated
void SwitchStmt::BuildTree(SymbolTable* symT) {
    for (int i = 0; i < cases->NumElements(); ++i)
        cases->Nth(i)->BuildTree(symT);

}
