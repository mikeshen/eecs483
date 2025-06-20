/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp3: You will need to extend the Decl classes to implement
 * semantic processing including detection of declaration conflicts
 * and managing scoping issues.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "ast_type.h"
#include "list.h"


class Type;
class NamedType;
class FnDecl;
class Identifier;
class Stmt;
class ImplementedFunction;

class Decl : public Node
{
protected:
    Identifier* id;

public:
    Decl(Identifier* name);
    friend std::ostream& operator<<(std::ostream& out, Decl* d) { return out << d->id; }
    virtual void BuildTree(SymbolTable* symT) { return; }
    virtual Type* getType() { return NULL; }
    virtual void Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT) {}
    char* getName() { return id->getName(); }
    Identifier* getIdentifier() { return id; }
};

class VarDecl : public Decl
{
protected:
    Type* type;

public:
    VarDecl(Identifier* name, Type* type);
    virtual void BuildTree(SymbolTable* symT);
    virtual Type* getType() { return type; }
	virtual void Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT);
};

class ClassDecl : public Decl
{
protected:
    NamedType* extends;
    List<NamedType*>* implements;
    SymbolTable* classScope;
    Hashtable<ImplementedFunction*>* implmentedFunctions;
    ClassDecl* parent;
    List<Decl*>* members;

    // new stuff
    Scoper* classScoper;
    Hashtable<FnDecl*>* vTable;
    List<FnDecl*>* emittedMethods;
    List<VarDecl*>* fields;
    int numFields;
    char* classLabel;

public:
    ClassDecl(Identifier* name, NamedType* extends,
    List<NamedType*>* implements, List<Decl*>* members);
    virtual void BuildTree(SymbolTable* symT);
    bool Inherit(SymbolTable* symT);
    bool FulfillsInterface(char* name);
    void EmitHelper(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT);
    virtual void Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT);
    Hashtable<FnDecl*>* getVTable() { return vTable; }
    List<VarDecl*>* getFields() { return fields; }
    Scoper* getScoper() { return classScoper; }
    int getNumFields() { return numFields; }
    char* getClassLabel() { return classLabel; }
};

class InterfaceDecl : public Decl
{
protected:
    List<Decl*>* members;
    SymbolTable* interfaceScope;

public:
    InterfaceDecl(Identifier* name, List<Decl*>* members);
    virtual void BuildTree(SymbolTable* symT);
    List<Decl*>* getMembers() { return members; }
};

class FnDecl : public Decl
{
protected:
    List<VarDecl*>* formals;
    Type* returnType;
    Stmt* body;
    SymbolTable* fnScope;

    Scoper* paramScoper;
    Scoper* bodyScoper;
    char* methodLabel;
    char* functionLabel;
    int offset;

public:
    FnDecl(Identifier* name, Type* returnType, List<VarDecl*>* formals);
    void SetFunctionBody(Stmt* b);
    virtual void BuildTree(SymbolTable* symT);
    void EmitMethod(ClassDecl* classDecl, Scoper* scoper, CodeGenerator* codegen, SymbolTable* symT);
    virtual void Emit(Scoper* scopee, CodeGenerator* codegen, SymbolTable* symT);

    virtual Type* getType() { return returnType; }
    List<VarDecl*>* getFormals() { return formals; }

    const char* getMethodLabel() { return methodLabel; }
    void setMethodLabel(char* label);
    const char* getFunctionLabel() { return functionLabel; }
    bool isMethod() { return methodLabel != NULL; }
    int getOffset() { return offset; }
    void setOffset(int off) { offset = off; }

};

class ImplementedFunction
{
protected:
    FnDecl* prototype;
    NamedType* intf_type;
    bool implemented;

public:
    ImplementedFunction(FnDecl* p, NamedType* type) : prototype(p), intf_type(type), implemented(false) {}
    FnDecl* getPrototype() { return prototype; }
    NamedType* getIntfType() { return intf_type; }
    bool isImplemented() { return implemented; }
    void setImplemented(bool imp) { implemented = imp; }
};

#endif
