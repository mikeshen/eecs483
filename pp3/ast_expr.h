/* File: ast_expr.h
 * ----------------
 * The Expr class and its subclasses are used to represent
 * expressions in the parse tree.  For each expression in the
 * language (add, call, New, etc.) there is a corresponding
 * node class for that construct.
 *
 * pp3: You will need to extend the Expr classes to implement
 * semantic analysis for rules pertaining to expressions.
 */


#ifndef _H_ast_expr
#define _H_ast_expr

#include "ast.h"
#include "ast_stmt.h"
#include "list.h"
#include "ast_decl.h"

class NamedType; // for new
class Type; // for NewArray


class Expr : public Stmt
{
public:
    Expr(yyltype loc) : Stmt(loc) {}
    Expr() : Stmt() {}
    virtual bool Check(SymbolTable* symT) { return true; }
    void setEvalType(Type* eType) { evaluatedType = eType; }
    Type* getEvalType() { return evaluatedType; }

protected:
    Type* evaluatedType;
};

/* This node type is used for those places where an expression is optional.
 * We could use a NULL pointer, but then it adds a lot of checking for
 * NULL. By using a valid, but no-op, node, we save that trouble */
 class EmptyExpr : public Expr
 {
 public:
    EmptyExpr();
    virtual bool Check(SymbolTable* symT) { return true; }
};

class IntConstant : public Expr
{
protected:
    int value;

public:
    IntConstant(yyltype loc, int val);
    virtual bool Check(SymbolTable* symT) { return true; }
};

class DoubleConstant : public Expr
{
protected:
    double value;

public:
    DoubleConstant(yyltype loc, double val);
    virtual bool Check(SymbolTable* symT) { return true; }
};

class BoolConstant : public Expr
{
protected:
    bool value;

public:
    BoolConstant(yyltype loc, bool val);
    virtual bool Check(SymbolTable* symT) { return true; }
};

class StringConstant : public Expr
{
protected:
    char* value;

public:
    StringConstant(yyltype loc, const char* val);
    virtual bool Check(SymbolTable* symT) { return true; }
};

class NullConstant: public Expr
{
public:
    NullConstant(yyltype loc);
    virtual bool Check(SymbolTable* symT) { return true; }
};

class Operator : public Node
{
protected:
    char tokenString[4];

public:
    Operator(yyltype loc, const char* tok);
    friend std::ostream& operator<<(std::ostream& out, Operator* o) { return out << o->tokenString; }
    virtual bool Check(SymbolTable* symT) { return true; }
};

class CompoundExpr : public Expr
{
protected:
    Operator* op;
    Expr* left,* right; // left will be NULL if unary

public:
    CompoundExpr(Expr* lhs, Operator* op, Expr* rhs); // for binary
    CompoundExpr(Operator* op, Expr* rhs);             // for unary
    virtual bool Check(SymbolTable* symT) {return true; }
};

class ArithmeticExpr : public CompoundExpr
{
public:
    ArithmeticExpr(Expr* lhs, Operator* op, Expr* rhs) : CompoundExpr(lhs,op,rhs) {}
    ArithmeticExpr(Operator* op, Expr* rhs) : CompoundExpr(op,rhs) {}
    bool Check(SymbolTable* symT);
};

class RelationalExpr : public CompoundExpr
{
public:
    RelationalExpr(Expr* lhs, Operator* op, Expr* rhs) : CompoundExpr(lhs,op,rhs) {}
    bool Check(SymbolTable* symT);
};

class EqualityExpr : public CompoundExpr
{
public:
    EqualityExpr(Expr* lhs, Operator* op, Expr* rhs) : CompoundExpr(lhs,op,rhs) {}
    bool Check(SymbolTable* symT);
};

class LogicalExpr : public CompoundExpr
{
public:
    LogicalExpr(Expr* lhs, Operator* op, Expr* rhs) : CompoundExpr(lhs,op,rhs) {}
    LogicalExpr(Operator* op, Expr* rhs) : CompoundExpr(op,rhs) {}
    bool Check(SymbolTable* symT);
};

class AssignExpr : public CompoundExpr
{
public:
    AssignExpr(Expr* lhs, Operator* op, Expr* rhs) : CompoundExpr(lhs,op,rhs) {}
    bool Check(SymbolTable* symT);
};

class LValue : public Expr
{
public:
    LValue(yyltype loc) : Expr(loc) {}
};

class This : public Expr
{
public:
    This(yyltype loc) : Expr(loc) {}
    bool Check(SymbolTable* symT);
};

class ArrayAccess : public LValue
{
protected:
    Expr* base,* subscript;

public:
    ArrayAccess(yyltype loc, Expr* base, Expr* subscript);
    bool Check(SymbolTable* symT);
};

/* Note that field access is used both for qualified names
 * base.field and just field without qualification. We don't
 * know for sure whether there is an implicit "this." in
 * front until later on, so we use one node type for either
 * and sort it out later. */
 class FieldAccess : public LValue
 {
 protected:
    Expr* base; // will be NULL if no explicit base
    Identifier* field;

public:
    FieldAccess(Expr* base, Identifier* field); //ok to pass NULL base
    bool Check(SymbolTable* symT);
    bool CheckBase(SymbolTable* symT);
};

/* Like field access, call is used both for qualified base.field()
 * and unqualified field().  We won't figure out until later
 * whether we need implicit "this." so we use one node type for either
 * and sort it out later. */
 class Call : public Expr
 {
 protected:
    Expr *base; // will be NULL if no explicit base
    Identifier* field;
    List<Expr*>* actuals;

public:
    Call(yyltype loc, Expr* base, Identifier* field, List<Expr*>* args);
    bool CheckBase(SymbolTable* symT);
    bool CheckCall(SymbolTable* symT, Symbol* sym);
    bool Check(SymbolTable* symT);
};

class NewExpr : public Expr
{
protected:
    NamedType* cType;

public:
    NewExpr(yyltype loc, NamedType* clsType);
    bool Check(SymbolTable* symT);
};

class NewArrayExpr : public Expr
{
protected:
    Expr* size;
    Type* elemType;

public:
    NewArrayExpr(yyltype loc, Expr* sizeExpr, Type* elemType);
    bool Check(SymbolTable* symT);
};

class ReadIntegerExpr : public Expr
{
public:
    ReadIntegerExpr(yyltype loc);
};

class ReadLineExpr : public Expr
{
public:
    ReadLineExpr(yyltype loc);
};

class PostfixExpr : public Expr
{
protected:
    LValue* lvalue;
    Operator* op;
public:
    PostfixExpr(LValue* lv, Operator* op);
};


#endif
