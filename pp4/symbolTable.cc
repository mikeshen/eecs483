#include "ast_decl.h"
#include "symbolTable.h"
/** Class: Symbol **/
Symbol::Symbol(E_Type t, Node* n) : type(t), node(n), env(NULL) {}

Symbol::Symbol(E_Type t, Node* n, Location* l) : type(t), node(n), loc(l) {}

Symbol::Symbol(E_Type t, Node* n, SymbolTable* e) : type(t), node(n), env(e) {}

/** Class: SymbolTable **/
SymbolTable::SymbolTable() : _parent(NULL), _super(NULL), _this(NULL),
                             ownernode(NULL), lastnode(NULL),
                             table(new Hashtable<Symbol*>),
                             blocks(new List<SymbolTable*>) {}

Node* SymbolTable::getThisClass() {
    if (_this == NULL)
        return NULL;
    return (_this)->getOwnerNode();
}

Symbol* SymbolTable::find(char* key) {
    Symbol* symbol = findLocal(key);
    if (symbol != NULL)
        return symbol;
    if (_parent == NULL)
        return NULL;
    return  _parent->find(key);
}


Symbol* SymbolTable::findLocal(char* key) {
    Symbol* symbol = table->Lookup(key);
    if (symbol != NULL)
        return symbol;
    if(_super == NULL)
        return NULL;
    return findSuper(key);
}

Symbol* SymbolTable::findSuper(char* key) {
    for (SymbolTable* current = _super ; current != NULL; current = current->getSuper()) {
        Symbol* symbol = current->table->Lookup(key);
        if (symbol != NULL)
            return symbol;
    }
    return NULL;
}

Symbol* SymbolTable::findClassField(char* className, char* fieldName) {
    if(className == NULL)
        return NULL;
    Symbol* classSymbol = find(className);
    if (classSymbol == NULL)
        return NULL;
    return classSymbol->getEnv()->find(fieldName);
}

bool SymbolTable::subclassOf(char* key) {
    for (SymbolTable* current = _super; current != NULL; current = current->getSuper()) {
        ClassDecl* classDecl = dynamic_cast<ClassDecl*>(current->getOwnerNode());
        if (classDecl == 0)
            continue; //////////////////////////////////////////////////////////////////////////////This part looks really shady
        if (strcmp(key, classDecl->getName()) == 0 || classDecl->FulfillsInterface(key))
            return true;
    }
    return false;
}

bool SymbolTable::add(char* key, Node* node) {
    Symbol* s = new Symbol(VARIABLE, node);
    table->Enter(key, s, false);
    return true;
}

bool SymbolTable::add(char* key, Node* node, Location* loc) {
    Symbol* s = new Symbol(VARIABLE, node, loc);
    table->Enter(key, s, false);
    return true;
}

SymbolTable* SymbolTable::addScope() {
    SymbolTable* child = new SymbolTable;
    child->setParent(this);
    if (_this)
        child->setThis(_this);
    blocks->Append(child);
    if (ownernode)
        child->setOwnerNode(ownernode);
    if (lastnode)
        child->setLastNode(lastnode);
    return child;
}

SymbolTable* SymbolTable::addUnderScope(char* key, Node* node, E_Type type) {
    SymbolTable* newEnv = new SymbolTable;
    Symbol* refNode = NULL;
    newEnv->setParent(this);
    if (_this)
        newEnv->setThis(_this);
    refNode = new Symbol(type, node, newEnv);
    table->Enter(key, refNode, false);
    newEnv->setOwnerNode(node);
    lastnode = NULL;
    return newEnv;
}
