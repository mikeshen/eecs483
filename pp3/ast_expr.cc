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
    evaluatedType = Type::voidType;
}

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc), value(val) {
    evaluatedType = Type::intType;
    }

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc), value(val) {
    evaluatedType = Type::doubleType;
    }

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc), value(val) {
    evaluatedType = Type::boolType;
    }

StringConstant::StringConstant(yyltype loc, const char* val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
    evaluatedType = Type::stringType;
}

NullConstant::NullConstant(yyltype loc) : Expr(loc) {
    evaluatedType = Type::nullType;
    }

Operator::Operator(yyltype loc, const char* tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

CompoundExpr::CompoundExpr(Expr* l, Operator* o, Expr* r)
    : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this);
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator* o, Expr* r)
    : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL;
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

bool ArithmeticExpr::Check(SymbolTable* symT) {
    std::cout << "ArithmeticExpr check reached" << std::endl;
    bool flag = true;

    flag = right->Check(symT) ? flag : false;
    Type* rightType = right->getEvalType();

    if (left != NULL) {
        flag = left->Check(symT) ? flag : false;
        Type* leftType = left->getEvalType();


        if (leftType->isConvertableTo(Type::intType) && rightType->isConvertableTo(Type::intType))
            setEvalType(Type::intType);
        else if (leftType->isConvertableTo(Type::doubleType) && rightType->isConvertableTo(Type::doubleType))
            setEvalType(Type::doubleType);
        else {
            flag = false;
            ReportError::IncompatibleOperands(op, leftType, rightType);
            setEvalType(Type::errorType);
        }
    }
    else {
        if (rightType->isConvertableTo(Type::intType) || rightType->isConvertableTo(Type::doubleType))
            setEvalType(rightType);
        else {
            flag = false;
            ReportError::IncompatibleOperand(op, rightType);
            setEvalType(Type::errorType);
        }
    }

    return flag;
}

bool RelationalExpr::Check(SymbolTable* symT) {
    std::cout << "RelationalExpr check reached" << std::endl;
    bool flag = left->Check(symT);
    Type* leftType = left->getEvalType();
    flag = right->Check(symT) ? flag : false;
    Type* rightType = right->getEvalType();
    if (leftType->isConvertableTo(rightType) && (rightType->isConvertableTo(Type::intType) || rightType->isConvertableTo(Type::doubleType))  ) ////May or may not be incorrect
        setEvalType(Type::boolType);
    else {
        flag = false;
        ReportError::IncompatibleOperands(op, leftType, rightType);
        setEvalType(Type::errorType);
    }
    return flag;
}

bool EqualityExpr::Check(SymbolTable* symT) {
    std::cout << "EqualityExpr check reached" << std::endl;
    bool flag = left->Check(symT);
    Type* leftType = left->getEvalType();
    flag = right->Check(symT) ? flag : false;
    Type* rightType = right->getEvalType();
    if (leftType->isConvertableTo(rightType)  || rightType->isConvertableTo(leftType)  )
        setEvalType(Type::boolType);
    else {
        flag = false;
        ReportError::IncompatibleOperands(op, leftType, rightType);
        setEvalType(Type::errorType);
    }
    return flag;
}

bool LogicalExpr::Check(SymbolTable* symT) {
    std::cout << "LogicalExpr check reached" << std::endl;
    bool flag = right->Check(symT);
    Type* rightType = right->getEvalType();


    if (left) {
        flag = left->Check(symT) ? flag : false;
        Type* leftType = left->getEvalType();


        if (leftType->isConvertableTo(Type::boolType) && rightType->isConvertableTo(Type::boolType))
            setEvalType(Type::boolType);
        else {
            ReportError::IncompatibleOperands(op, leftType, rightType);
            setEvalType(Type::errorType);
            flag = false;
        }
    }
    else {
        if (rightType->isConvertableTo(Type::boolType))
            setEvalType(Type::boolType);
        else {
            flag = false;
            ReportError::IncompatibleOperand(op, rightType);
            setEvalType(Type::errorType);
        }
    }
    return flag;
}

bool AssignExpr::Check(SymbolTable* symT) {
    std::cout << "AssignExpr check reached" << std::endl;
    bool flag = left->Check(symT);
    flag = right->Check(symT) ? flag : false;

    Type* leftType = left->getEvalType();
    Type* rightType = right->getEvalType();

    if (leftType->isEquivalentTo(Type::errorType) || rightType->isConvertableTo(leftType))
        setEvalType(leftType);
    else {
        flag = false;
        ReportError::IncompatibleOperands(op, leftType, rightType);
        setEvalType(Type::errorType);
    }
    return flag;
}


bool This::Check(SymbolTable* symT) {
    std::cout << "This check reached" << std::endl;
    Node* node = symT->getThisClass();
    if (node == NULL) {
        ReportError::ThisOutsideClassScope(this);
        setEvalType(Type::errorType);
        return false;
    }

    ClassDecl* currentClass = static_cast<ClassDecl*>(node);
    //Assert(currentClass != 0);
    setEvalType(new NamedType(currentClass->getIdentifier()));
    return true;
}


ArrayAccess::ArrayAccess(yyltype loc, Expr* b, Expr* s) : LValue(loc) {
        (base=b)->SetParent(this);
        (subscript=s)->SetParent(this);
}

bool ArrayAccess::Check(SymbolTable* symT) {
    std::cout << "ArrayAccess check reached" << std::endl;
    std::cout << "checked" << std::endl;
    bool flag = true;
    flag = base->Check(symT) ? flag : false;
    flag = subscript->Check(symT) ? flag : false;

    if (!(subscript->getEvalType()->isConvertableTo(Type::intType))  ) {
        flag = false;
        ReportError::SubscriptNotInteger(subscript);
        setEvalType(Type::errorType);
    }

    ArrayType* baseType = dynamic_cast<ArrayType*>(base->getEvalType());
    if (baseType == 0) {
        flag = false;
        ReportError::BracketsOnNonArray(base);
        setEvalType(Type::errorType);
    }
    else if(getEvalType()->isEquivalentTo(Type::errorType) )
        setEvalType(baseType->getElemType());

    return flag;
}

FieldAccess::FieldAccess(Expr* b, Identifier* f)
    : LValue(b? Join(b->GetLocation(), f->GetLocation()) :* f->GetLocation()) {
        Assert(f != NULL); // b can be be NULL (just means no explicit base)
        base = b;
        if (base) base->SetParent(this);
        (field=f)->SetParent(this);
}

bool FieldAccess::CheckBase(SymbolTable* symT) {
    NamedType* baseType = dynamic_cast<NamedType*>(base->getEvalType()); //////////////////////////Not sure if this part is correct
    if (baseType == 0) {
        ReportError::FieldNotFoundInBase(field, base->getEvalType());
        setEvalType(Type::errorType);
        return false;
    }

    Symbol* element = symT->findClassField(baseType->getName(), field->getName(), VARIABLE);
    if (element == NULL) {
        ReportError::FieldNotFoundInBase(field, baseType);
        setEvalType(Type::errorType);
        return false;
    }


    Node* currentClass = symT->getThisClass();
    if (currentClass == NULL) {
        ReportError::InaccessibleField(field, base->getEvalType());
        setEvalType(Type::errorType);
        return false;
    }
    ClassDecl* classDecl = static_cast<ClassDecl*>(currentClass);
    //Assert(classDecl != 0);
    if (dynamic_cast<This*>(base) != 0) {
        FieldAccess* baseFieldAccess = static_cast<FieldAccess*>(base);
        //Assert(baseFieldAccess != 0);
        if (strcmp(classDecl->getIdentifier()->getName(), base->getEvalType()->getName()) != 0) {
            ReportError::InaccessibleField(field, base->getEvalType());
            setEvalType(Type::errorType);
            return false;
        }
    }
    Decl* fieldDecl = static_cast<Decl*>(element->getNode());
    //Assert(fieldDecl != 0);
    setEvalType(fieldDecl->getType());

    return true;
}

bool FieldAccess::Check(SymbolTable* symT) {
    std::cout << "Field Access check reached" << std::endl;
    bool flag = true;
    if (base == NULL) {
        Symbol* sym = symT->find(field->getName(), VARIABLE);
        if (sym == NULL) {
            ReportError::IdentifierNotDeclared(field, LookingForVariable);
            setEvalType(Type::errorType);
            return false;
        }

        setEvalType((static_cast<VarDecl*>(sym->getNode()))->getType());
        return true;
    }
    flag = base->Check(symT) ? flag : false;
    flag = CheckBase(symT) ? flag : false;

    return flag;
}


Call::Call(yyltype loc, Expr* b, Identifier* f, List<Expr*>* a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}


NewExpr::NewExpr(yyltype loc, NamedType* c) : Expr(loc) {
Assert(c != NULL);
(cType=c)->SetParent(this);
}


NewArrayExpr::NewArrayExpr(yyltype loc, Expr* sz, Type* et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this);
    (elemType=et)->SetParent(this);
}

PostfixExpr::PostfixExpr(LValue* lv, Operator* o) : Expr(Join(lv->GetLocation(), o->GetLocation())) {
    Assert(lv != NULL && o != NULL);
    (lvalue=lv)->SetParent(this);
    (op=o)->SetParent(this);
}

