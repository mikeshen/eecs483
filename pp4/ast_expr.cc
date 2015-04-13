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
    if(string(op->getTokenString()) == "<")
        framePosition = codegen->GenBinaryOp(op->getTokenString(), left->getFramePosition(), right->getFramePosition(), scopee);
    if(string(op->getTokenString()) == ">")
        framePosition = codegen->GenBinaryOp("<", right->getFramePosition(), left->getFramePosition(), scopee);
    if(string(op->getTokenString()) == "<=") {
        framePosition = codegen->GenBinaryOp("<", right->getFramePosition(), left->getFramePosition(), scopee);
        framePosition = codegen->GenBinaryOp("==", framePosition, codegen->GenLoadConstant(0, scopee), scopee);
    }
    if(string(op->getTokenString()) == ">=") {
        framePosition = codegen->GenBinaryOp("<", left->getFramePosition(), right->getFramePosition(), scopee);
        framePosition = codegen->GenBinaryOp("==", framePosition, codegen->GenLoadConstant(0, scopee), scopee);
    }
}

void EqualityExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    left->Emit(scopee, codegen, symT);
    right->Emit(scopee, codegen, symT);
    Type* leftType = left->getEvalType(symT);
    Type* rightType = right->getEvalType(symT);
    if(leftType->IsEquivalentTo(Type::stringType) && rightType->IsEquivalentTo(Type::stringType)) {
        framePosition = codegen->GenBuiltInCall(StringEqual, scopee, left->getFramePosition(), right->getFramePosition());
        if(strcmp(op->getTokenString(), "!=") == 0)
            framePosition = codegen->GenBinaryOp("==", framePosition, codegen->GenLoadConstant(0, scopee), scopee);
        return;
    }
    if (strcmp(op->getTokenString(), "!=") == 0) {
        framePosition = codegen->GenBinaryOp("==", left->getFramePosition(), right->getFramePosition(), scopee);
        framePosition = codegen->GenBinaryOp("==", framePosition, codegen->GenLoadConstant(0, scopee), scopee);
    }
    else
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
    if(left->getDeref())
        codegen->GenStore(left->getRef(), right->getFramePosition(), 0);
    else
        codegen->GenAssign(left->getFramePosition(), right->getFramePosition());

    framePosition = left->getFramePosition();
}

void This::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    framePosition = symT->find(strdup("this"), VARIABLE)->getLoc();
}

void ArrayAccess::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT){
    base->Emit(scopee, codegen, symT);
    subscript->Emit(scopee, codegen, symT);
    Location* zero = codegen->GenLoadConstant(0, scopee);
    Location* lowerBound = codegen->GenBinaryOp("<", subscript->getFramePosition(), zero, scopee);
    Location* maxIndex = codegen->GenLoad(base->getFramePosition(), scopee, -4);
    Location* upperBound = codegen->GenBinaryOp("<", subscript->getFramePosition(), maxIndex, scopee);
    Location* flip = codegen->GenBinaryOp("==", zero, upperBound, scopee);
    Location* theTest = codegen->GenBinaryOp("||", flip, lowerBound, scopee);
    char* endPoint = codegen->NewLabel();
    codegen->GenIfZ(theTest, endPoint);
    Location* errorMessage = codegen->GenLoadConstant(err_arr_out_of_bounds, scopee);
    codegen->GenBuiltInCall(PrintString, scopee, errorMessage, NULL);
    codegen->GenBuiltInCall(Halt, scopee, NULL, NULL);
    codegen->GenLabel(endPoint);
    Location* four = codegen->GenLoadConstant(4, scopee);
    Location* offset = codegen->GenBinaryOp("*", four, subscript->getFramePosition(), scopee);

    reference = codegen->GenBinaryOp("+", base->getFramePosition(), offset, scopee);

    framePosition = codegen->GenLoad(reference, scopee, 0); //TODO: check
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

Type* FieldAccess::getEvalType(SymbolTable* symT) { ////////////////////////////////////////////////////Fix for classes
    if(base) {
        NamedType* baseType = dynamic_cast<NamedType*>(base->getEvalType(symT));
        Symbol* element = symT->findClassField(baseType->getName(), field->getName(), VARIABLE);
        Decl *fieldDecl = static_cast<Decl*>(element->getNode());
        return fieldDecl->getType();
    }
    Symbol* sym = symT->find(field->getName(), VARIABLE);
    return (static_cast<VarDecl*>(sym->getNode()))->getType();
}

void FieldAccess::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    Location* loc = NULL;
    Location* temp2 = NULL;

    if(base) {
        base->Emit(scopee, codegen, symT);
        Symbol* klass = symT->find(base->getEvalType(symT)->getName(), CLASS);
        Symbol* fieldSym = klass->getEnv()->find(field->getName(), VARIABLE);
        loc = fieldSym->getLoc();
        temp2 = base->getFramePosition();
    }
    else {
        Symbol* sym = symT->find(field->getName(), VARIABLE);
        Symbol* temp = symT->find(strdup("this"), VARIABLE);
        loc = sym->getLoc();
        if(loc->GetSegment() != cRelative) {
            framePosition = loc;
            return;
        }
        temp2 = temp->getLoc();
    }

        Assert(temp2);

    deref = true;
    Location* offset = codegen->GenLoadConstant(loc->GetOffset(), scopee);
    reference = codegen->GenBinaryOp("+", temp2, offset, scopee);
    framePosition = codegen->GenLoad(reference, scopee, 0);
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

int Call::EmitParams(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    int numParams = 0;
    for (int i = 0; i < actuals->NumElements(); ++i) {
        actuals->Nth(i)->Emit(scopee, codegen, symT);
        ++numParams;
    }
    for (int i = actuals->NumElements() - 1; i >= 0; --i)
        codegen->GenPushParam(actuals->Nth(i)->getFramePosition());
    return numParams;
}

void Call::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    Symbol* functionSymbol = NULL;
    FnDecl* fnDecl = NULL;
    bool isNonVoid = true;
    int numParams = 0;
    int offset = 0;
    Location* objectPosition = NULL;
    Location* methodAddress = NULL;

    if (!base) {
        functionSymbol = symT->find(field->getName(), FUNCTION);
        fnDecl = dynamic_cast<FnDecl*>(functionSymbol->getNode());

        if (fnDecl->getType()->IsEquivalentTo(Type::voidType))
            isNonVoid = false;
        if (!fnDecl->isMethod()) {
            numParams += EmitParams(scopee, codegen, symT);
            char* functionLabel = codegen->NewFunctionLabel(fnDecl->getName());
            framePosition = codegen->GenLCall(functionLabel, isNonVoid, scopee);
            codegen->GenPopParams(numParams * 4);
            return;
        }


        Symbol* thisSymbol = symT->find(strdup("this"));
        objectPosition = thisSymbol->getLoc();
        offset = fnDecl->getOffset();
        Location* temporary = codegen->GenLoad(thisSymbol->getLoc(), scopee, 0);
        methodAddress = codegen->GenLoad(temporary, scopee, offset * 4);
    }
    else {
        base->Emit(scopee, codegen, symT);
        if (dynamic_cast<ArrayType*>(base->getEvalType(symT)) != 0 &&
            strcmp(field->getName(), "length") == 0) {
            framePosition = codegen->GenLoad(base->getFramePosition(), scopee, -4);
            return;
        }

        Symbol* classSymbol = symT->find(base->getEvalType(symT)->getName(), CLASS);
        functionSymbol = classSymbol->getEnv()->find(field->getName(), FUNCTION);
        fnDecl = dynamic_cast<FnDecl*>(functionSymbol->getNode());
        if (fnDecl->getType()->IsEquivalentTo(Type::voidType))
            isNonVoid = false;

        objectPosition = base->getFramePosition();
        offset = fnDecl->getOffset();
        methodAddress = codegen->GenLoad(codegen->GenLoad(base->getFramePosition(), scopee, 0), scopee, offset * 4);
    }

    numParams += EmitParams(scopee, codegen, symT);
    ++numParams;
    codegen->GenPushParam(objectPosition);
    framePosition = codegen->GenACall(methodAddress, isNonVoid, scopee);
    codegen->GenPopParams(numParams * 4);
}

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) {
    Assert(c != NULL);
    (cType=c)->SetParent(this);
}


void NewExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    Location* four = codegen->GenLoadConstant(4, scopee);
    ClassDecl* classDecl = dynamic_cast<ClassDecl*>((symT->find(cType->getName(), CLASS))->getNode());
    Location* classSize = codegen->GenBinaryOp("*", codegen->GenLoadConstant(classDecl->getNumFields() + 1, scopee), four, scopee);
    framePosition = codegen->GenBuiltInCall(Alloc, scopee, classSize, NULL);
    codegen->GenStore(framePosition, codegen->GenLoadLabel(classDecl->getClassLabel(), scopee), 0);
}


NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this);
    (elemType=et)->SetParent(this);
}


void NewArrayExpr::Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {
    size->Emit(scopee, codegen, symT);

    Location* one = codegen->GenLoadConstant(1, scopee);
    Location* sizeCheck = codegen->GenBinaryOp("<", size->getFramePosition(), one, scopee);
    
    char* endPoint = codegen->NewLabel();
    codegen->GenIfZ(sizeCheck, endPoint);
    Location* errorMessage = codegen->GenLoadConstant(err_arr_bad_size, scopee);
    codegen->GenBuiltInCall(PrintString, scopee, errorMessage, NULL);
    codegen->GenBuiltInCall(Halt, scopee, NULL, NULL);
    codegen->GenLabel(endPoint);
    Location* four = codegen->GenLoadConstant(4, scopee);
    Location* arrayDimension = codegen->GenBinaryOp("*", four, codegen->GenBinaryOp("+", one, size->getFramePosition(), scopee), scopee);
    Location* start = codegen->GenBuiltInCall(Alloc, scopee, arrayDimension, NULL);
    codegen->GenStore(start, size->getFramePosition(), 0);
    framePosition = codegen->GenBinaryOp("+", four, start, scopee);
}


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
