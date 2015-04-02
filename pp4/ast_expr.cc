/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>
#include "errors.h"



EmptyExpr::EmptyExpr() {
}

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

NullConstant::NullConstant(yyltype loc) : Expr(loc) {
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}
CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r)
    : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this);
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r)
    : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL;
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

void ArithmeticExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    right->Emit(scopee, codegen, symT);
    if(!left)
        left = new IntConstant({0,0,0,0,0, nullptr }, 0);
    left->Emit(scopee, codegen, symT);
    framePosition = codegen->GenBinaryOp(op->getTokenString(), left->getFramePosition(), right->getFramePosition(), scopee);
}

void RelationalExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    left->Emit(scopee, codegen, symT);
    right->Emit(scopee, codegen, symT);
    framePosition = codegen->GenBinaryOp(op->getTokenString(), left->getFramePosition(), right->getFramePosition(), scopee);
}

void EqualityExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    left->Emit(scopee, codegen, symT);
    right->Emit(scopee, codegen, symT);
    if(left->getEvalType(symT)->IsEquivalentTo(Type::stringType) && right->getEvalType(symT)->IsEquivalentTo(Type::stringType))
        framePosition = codegen->GenBuiltInCall(StringEqual, scopee, left->getFramePosition(), right->getFramePosition());
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// != for string
    framePosition = codegen->GenBinaryOp(op->getTokenString(), left->getFramePosition(), right->getFramePosition(), scopee);
}

ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
        (base=b)->SetParent(this);
        (subscript=s)->SetParent(this);
}

FieldAccess::FieldAccess(Expr *b, Identifier *f)
    : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
}

Type* FieldAccess::getEvalType(SymbolTable* symT) {
    if(!base) {
        NamedType* baseType = dynamic_cast<NamedType*>(base->getEvalType(symT));
        Symbol* element = symT->findClassField(baseType->getName(), field->getName(), VARIABLE);
        Decl *fieldDecl = static_cast<Decl*>(element->getNode());
        return fieldDecl->getType();
    }
    Symbol* sym = symT->find(field->getName(), VARIABLE);
    return (static_cast<VarDecl*>(sym->getNode()))->getType();
}



Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
        Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
        base = b;
        if (base) base->SetParent(this);
        (field=f)->SetParent(this);
        (actuals=a)->SetParentAll(this);
}

Type* Call::getEvalType(SymbolTable* symT) {
    if(!base) {
        Symbol* sym = symT->find(field->getName(), FUNCTION);
        return (dynamic_cast<FnDecl*>(sym->getNode()))->GetReturnType();
    }
    if (dynamic_cast<ArrayType*>(base->getEvalType(symT)) != 0 && strcmp(field->getName(), "length") == 0)
        return Type::intType;

    Type* basetype = base->getEvalType(symT);
    Symbol* sym = symT->findClassField(basetype->getName(), field->getName(), FUNCTION);
    return (dynamic_cast<FnDecl*>(sym->getNode()))->GetReturnType();
}

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) {
    Assert(c != NULL);
    (cType=c)->SetParent(this);
}


NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this);
    (elemType=et)->SetParent(this);
}


ReadIntegerExpr::ReadIntegerExpr(yyltype loc) : Expr(loc) {
}

ReadLineExpr::ReadLineExpr(yyltype loc) : Expr (loc) {
}

PostfixExpr::PostfixExpr(LValue *lv, Operator *o) : Expr(Join(lv->GetLocation(), o->GetLocation())) {
    Assert(lv != NULL && o != NULL);
    (lvalue=lv)->SetParent(this);
    (op=o)->SetParent(this);
}

