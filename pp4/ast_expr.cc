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


void LogicalExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    right->Emit(scopee, codegen, symT);
    if(!left){
        left = new BoolConstant({0,0,0,0,0, nullptr }, 0);
        delete op;
        op = new Operator({0,0,0,0,0, nullptr }, "==");
    }
    left->Emit(scopee, codegen, symT);
    framePosition = codegen->GenBinaryOp(op->getTokenString(), left->getFramePosition(), right->getFramePosition(), scopee);
}


void AssignExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    right->Emit(scopee, codegen, symT);
    left->Emit(scopee, codegen, symT);
    if(deref)
        codegen->GenStore(left->getFramePosition(), right->getFramePosition(), 0);
    else
        codegen->GenAssign(left->getFramePosition(), right->getFramePosition());////////////I have no idea when this is actually used
    framePosition = left->getFramePosition();
}

void This::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    framePosition = (symT->find("this"))->getLoc();//////////////////////////////////////////////////////
}

void ArrayAccess::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT){
    base->Emit(scopee, codegen, symT);
    subscript->Emit(scopee, codegen, symT);
    Location* minIndex = codegen->GenLoadConstant(0, scopee);
    Location* lowerBound = codegen->GenBinaryOp("<", subscript->getFramePosition(), minIndex, scopee);
    Location* maxIndex = codegen->GenLoad(base->getFramePosition(), scopee, 0);
    Location* upperBound = codegen->GenBinaryOp(">=", subscript->getFramePosition(), maxIndex, scopee);

    Location* theTest = codegen->GenBinaryOp("||", upperBound, lowerBound, scopee);
    char* endPoint = codegen->NewLabel();
    codegen->GenIfZ(theTest, endPoint);
    ////////////////////////////////No error message
    codegen->GenLabel(endPoint);
    Location* four = codegen->GenLoadConstant(4, scopee);
    Location* one = codegen->GenLoadConstant(1, scopee);
    Location* temp = codegen->GenBinaryOp("+", subscript->getFramePosition(), one, scopee);
    Location* offset = codegen->GenBinaryOp("*", temp, subscript->getFramePosition(), scopee);
    framePosition = codegen->GenLoad(codegen->GenBinaryOp("+", base->getFramePosition(), offset, scopee), scopee, 0);
}


ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
        (base=b)->SetParent(this);
        (subscript=s)->SetParent(this);
        deref = true;
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

void FieldAccess::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    if(base) {
        deref = true;
        base->Emit(scopee, codegen, symT);
        Symbol* klass = symT->find(base->getEvalType(symT)->getName(), CLASS);
        Symbol* fieldSym = klass->getEnv()->find(field->getName(), VARIABLE);
        Location* loc = fieldSym->getLoc();
        Location* offset = codegen->GenLoadConstant(loc->GetOffset(), scopee);
        framePosition = codegen->GenLoad(codegen->GenBinaryOp("+", loc, offset, scopee), scopee, 0);
        return;
    }

    Symbol* sym = symT->find(field->getName(), VARIABLE);
    Location* loc = sym->getLoc();
    /*
    if(loc->getSegment() != classRelative) {
        framePosition = loc
        return;
    }
    */
    deref = true;
    Symbol* temp = symT->find("this", VARIABLE);
    Location* offset = codegen->GenLoadConstant(loc->GetOffset(), scopee);
    framePosition = codegen->GenLoad(codegen->GenBinaryOp("+", temp->getLoc(), offset, scopee), scopee, 0);
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
        return (dynamic_cast<FnDecl*>(sym->getNode()))->getType();
    }
    if (dynamic_cast<ArrayType*>(base->getEvalType(symT)) != 0 && strcmp(field->getName(), "length") == 0)
        return Type::intType;

    Type* basetype = base->getEvalType(symT);
    Symbol* sym = symT->findClassField(basetype->getName(), field->getName(), FUNCTION);
    return (dynamic_cast<FnDecl*>(sym->getNode()))->getType();
}

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) {
    Assert(c != NULL);
    (cType=c)->SetParent(this);
}

/*
void NewExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    Location* four = codegen->GenLoadConstant(4, scopee);
    ClassDecl* class_decl = dynamic_cast<ClassDecl*>((symT->find(cType->getName(), CLASS))->getNode());
    Location* classSize = codegen->GenBinaryOp("*", codegen->GenLoadConstant(falloc, class_decl->NumFields() + 1), four, scopee);
    framePosition = codegen->GenBuiltInCall(falloc, Alloc, classSize, NULL);

    codegen->GenStore(framePosition, codegen->GenLoadLabel(falloc, class_decl->GetClassLabel()), 0);

}
*/

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this);
    (elemType=et)->SetParent(this);
}

/*
void Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    ClassDecl* klass = dynamic_cast<ClassDecl*>((symT->find(cType->getName(), CLASS);)->getNode());
    Location* four = codegen->GenLoadConstant(4, scopee);
    Location* temp = codegen->GenLoadConstant(klass->NumFields)
    Location* size = codegen->GenBinaryOp("*", )
}
*/

ReadIntegerExpr::ReadIntegerExpr(yyltype loc) : Expr(loc) {}

void ReadIntegerExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    framePosition = codegen->GenBuiltInCall(ReadInteger, scopee, NULL, NULL);
}

ReadLineExpr::ReadLineExpr(yyltype loc) : Expr (loc) {}

void ReadLineExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    framePosition = codegen->GenBuiltInCall(ReadLine, scopee, NULL, NULL);
}

PostfixExpr::PostfixExpr(LValue *lv, Operator *o) : Expr(Join(lv->GetLocation(), o->GetLocation())) {
    Assert(lv != NULL && o != NULL);
    (lvalue=lv)->SetParent(this);
    (op=o)->SetParent(this);
}

