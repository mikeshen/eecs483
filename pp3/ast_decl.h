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

class Identifier;
class Stmt;

class Decl : public Node
{
  protected:
    Identifier* id;

  public:
    Decl(Identifier* name);
    friend std::ostream& operator<<(std::ostream& out, Decl* d) { return out << d->id; }
    virtual bool BuildTree(SymbolTable* symT) { return true; }
    virtual bool Check(SymbolTable* symT) { return true; }
    virtual Type* getType() { return NULL; }
    char* getName() { return id->getName(); }
    Identifier* getIdentifier() { return id; }
};

class VarDecl : public Decl
{
  protected:
    Type* type;

  public:
    VarDecl(Identifier* name, Type* type);
    virtual bool BuildTree(SymbolTable* symT);
    virtual bool Check(SymbolTable* symT);
    virtual Type* getType() { return type; }
};

class ClassDecl : public Decl
{
  protected:
    List<Decl*>* members;
    NamedType* extends;
    List<NamedType*>* implements;
    SymbolTable* classScope;

  public:
    ClassDecl(Identifier* name, NamedType* extends,
              List<NamedType*>* implements, List<Decl*>* members);
    bool ImplementsInterface(char* name);
    virtual bool BuildTree(SymbolTable* symT);
    virtual bool Check(SymbolTable* symT);
};

class InterfaceDecl : public Decl
{
  protected:
    List<Decl*>* members;
    SymbolTable* interfaceScope;

  public:
    InterfaceDecl(Identifier* name, List<Decl*>* members);
    virtual bool BuildTree(SymbolTable* symT);
    virtual bool Check(SymbolTable* symT);
};

class FnDecl : public Decl
{
  protected:
    List<VarDecl*>* formals;
    Type* returnType;
    Stmt* body;
    SymbolTable* fnScope;

  public:
    FnDecl(Identifier* name, Type* returnType, List<VarDecl*>* formals);
    void SetFunctionBody(Stmt* b);
    virtual bool BuildTree(SymbolTable* symT);
    virtual bool Check(SymbolTable* symT);
    Type* GetReturnType() { return returnType; }
    Type* GetType() { return returnType; }
    List<VarDecl*>* GetFormals() { return formals; }
};

#endif
