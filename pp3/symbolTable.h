#ifndef _H_SymbolTable
#define _H_SymbolTable
#include "ast.h"
#include "list.h"
#include "hashtable.h"

enum E_Type {
    VARIABLE,
    INTERFACE,
    FUNCTION,
    CLASS
};

class SymbolTable;

class Symbol {
public:
    Symbol(E_Type t, Node* n);

    Symbol(E_Type t, Node* n, SymbolTable* e);

    E_Type getType() {
        return type;
    }

    void setType(E_Type t) {
        type = t;
    }

    Node* getNode() {
        return node;
    }

    void setNode(Node* n) {
        node = n;
    }

    SymbolTable* getEnv() {
        return env;
    }

    void setEnv(SymbolTable* e) {
        env = e;
    }
protected:
    E_Type type;
    Node* node;
    SymbolTable* env;
};

class SymbolTable {
protected:
    SymbolTable* _parent;
    SymbolTable* _super;
    SymbolTable* _this;
    Node* ownernode;
    Node* lastnode;
    Hashtable<Symbol*>* table;
    List<SymbolTable*>* blocks;
public:
    SymbolTable();

    void setParent(SymbolTable* p) {
        _parent = p;
    }

    SymbolTable* getParent() {
        return _parent;
    }

    void setSuper(SymbolTable* s) {
        _super = s;
    }

    SymbolTable* getSuper() {
        return _super;
    }

    void setThis(SymbolTable* t) {
        _this = t;
    }

    SymbolTable* getThis() {
        return _this;
    }

    void setOwnerNode(Node* s) {
        ownernode = s;
    }

    Node* getOwnerNode() {
        return ownernode;
    }

    void setLastNode(Node* s) {
        lastnode = s;
    }

    Node* getLastNode() {
        return lastnode;
    }

    Node* getThisClass();

    Symbol* find(char* key);

    Symbol* find(char* key, E_Type type) {
        Symbol* s = find(key);
        return (s && s->getType() == type) ? s : NULL;
    }

    Symbol* findLocal(char* key);

    Symbol* findLocal(char* key, E_Type type) {
        Symbol* s = findLocal(key);
        return (s && s->getType() == type) ? s : NULL;
    }

    Symbol* findInClass(char* key);

    Symbol* findInClass(char* key, E_Type type) {
        Symbol* s = findInClass(key);
        return (s && s->getType() == type) ? s : NULL;
    }

    Symbol* findUp(char* key);

    Symbol* findUp(char* key, E_Type type) {
        Symbol* s = findUp(key);
        return (s && s->getType() == type) ? s : NULL;
    }

    Symbol* findSuper(char* key);

    Symbol* findSuper(char* key, E_Type type) {
        Symbol* s = findSuper(key);
        return (s && s->getType() == type) ? s : NULL;
    }

    Symbol* findClassField(char* className, char* fieldName);

    Symbol* findClassField(char* className, char* fieldName, E_Type type) {
        Symbol* s = findClassField(className, fieldName);
        return (s && s->getType() == type) ? s : NULL;
    }

    bool subclassOf(char* key);

    bool add(char* key, Node* node);

    SymbolTable* addScope();

    SymbolTable* addUnderScope(char* key, Node* node, E_Type type);

    int getSize();
};

#endif
