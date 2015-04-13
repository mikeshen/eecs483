/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct.
 *
 * pp3: You will need to extend the Stmt classes to implement
 * semantic analysis for rules pertaining to statements.
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include "list.h"
#include "ast.h"
#include <sstream>

 class Decl;
 class VarDecl;
 class Expr;

 class Program : public Node
 {
 protected:
    SymbolTable* programScope;
    List<Decl*>* decls;
    CodeGenerator* codegen;
    Scoper* scoper;
public:
    Program(List<Decl*>* declList);
    void Check();
    void Emit();
};

class Stmt : public Node
{
public:
    Stmt() : Node() {}
    Stmt(yyltype loc) : Node(loc) { frameLoc = NULL;}
    virtual void BuildTree(SymbolTable* symT) { return; }
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT) {}
	Location* getFrameLoc() { return frameLoc; }

protected:
	Location* frameLoc;
};

class StmtBlock : public Stmt
{
protected:
    List<VarDecl*>* decls;
    List<Stmt*>* stmts;
    SymbolTable* blockScope;

public:
    StmtBlock(List<VarDecl*>* variableDeclarations, List<Stmt*>* statements);
    virtual void BuildTree(SymbolTable* symT);
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};


class ConditionalStmt : public Stmt
{
protected:
    Expr* test;
    Stmt* body;
    SymbolTable* blockScope;

public:
    ConditionalStmt(Expr* testExpr, Stmt* body);
};

class LoopStmt : public ConditionalStmt
{
public:
    LoopStmt(Expr* testExpr, Stmt* body)
    : ConditionalStmt(testExpr, body), nextLabel(NULL) {}

    char* GetNextLabel() { return nextLabel; }
protected:
    char* nextLabel;
};

class ForStmt : public LoopStmt
{
protected:
    Expr* init,* step;

public:
    ForStmt(Expr* init, Expr* test, Expr* step, Stmt* body);
    virtual void BuildTree(SymbolTable* symT);
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};

class WhileStmt : public LoopStmt
{
public:
    WhileStmt(Expr* test, Stmt* body) : LoopStmt(test, body) {}
    virtual void BuildTree(SymbolTable* symT);
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};

class IfStmt : public ConditionalStmt
{
protected:
    Stmt* elseBody;

public:
    IfStmt(Expr* test, Stmt* thenBody, Stmt* elseBody);
    virtual void BuildTree(SymbolTable* symT);
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};

class BreakStmt : public Stmt
{
public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};

class ReturnStmt : public Stmt
{
protected:
    Expr* expr;

public:
    ReturnStmt(yyltype loc, Expr* expr);
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};

class PrintStmt : public Stmt
{
protected:
    List<Expr*>* args;

public:
    PrintStmt(List<Expr*>* arguments);
    bool isPrintable(Type* type);
	virtual void Emit(Scoper *scopee, CodeGenerator *codegen, SymbolTable* symT);
};


class IntConstant;

class Case : public Node
{
protected:
    IntConstant* value;
    List<Stmt*>* stmts;
    SymbolTable* caseScope;

public:
    Case(IntConstant* v, List<Stmt*>* stmts);
    virtual void BuildTree(SymbolTable* symT);
};

class SwitchStmt : public Stmt
{
protected:
    Expr* expr;
    List<Case*>* cases;

public:
    SwitchStmt(Expr* e, List<Case*>* cases);
    virtual void BuildTree(SymbolTable* symT);
};

Type* convertArray(Type* type);
#endif
