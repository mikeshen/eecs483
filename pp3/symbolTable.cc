#include "ast_decl.h"
#include "symbolTable.h"
/** Class: Symbol **/
Symbol::Symbol(E_Type t, Node* n) : type(t), node(n), env(NULL) {}

Symbol::Symbol(E_Type t, Node* n, SymbolTable* e) : type(t), node(n), env(e) {}

/** Class: SymbolTable **/
SymbolTable::SymbolTable() : _parent(NULL), _super(NULL), _this(NULL),
                             ownernode(NULL), lastnode(NULL),
                             table(new Hashtable<Symbol*>),
                             blocks(new List<SymbolTable*>) {}

Node* SymbolTable::getThisClass() {
    if (!_this) return NULL;
    SymbolTable* temp = _this;
    return temp->getOwnerNode();
}

Symbol* SymbolTable::find(char* key) {
    SymbolTable* current = this;
    Symbol* symbol = NULL;
    for ( ; current != NULL; current = current->getParent())
        if ((symbol = current->findLocal(key)) != NULL)
            return symbol;
    if (_super && (symbol = findSuper(key)) != NULL) return symbol;
    return NULL;
}

Symbol* SymbolTable::findLocal(char* key) {
    Symbol* symbol = NULL;
    if ((symbol = table->Lookup(key)) != NULL) return symbol;
    if (_super && (symbol = findSuper(key)) != NULL) return symbol;
    return NULL;
}

Symbol* SymbolTable::findInClass(char* key) {
    Symbol* symbol = NULL;
    if ((symbol = findLocal(key)) != NULL) return symbol;
    if ((symbol = findSuper(key)) != NULL)  return symbol;
    return NULL;
}

Symbol* SymbolTable::findUp(char* key) {
    SymbolTable* current = _parent;
    for ( ; current != NULL; current = current->getParent()) {
        Symbol* symbol;
        if ((symbol = current->findLocal(key)) != NULL)
            return symbol;
    }
    return NULL;
}

Symbol* SymbolTable::findSuper(char* key) {
    SymbolTable* current = _super;
    if (!_super) return NULL;
    for ( ; current != NULL; current = current->getSuper()) {
        Symbol* symbol;
        if ((symbol = current->findLocal(key)) != NULL)
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
/*    SymbolTable* current = _super;
    ClassDecl* classDecl = NULL;
    if (!_super) {
        return false;
    }
    for ( ; current != NULL; current = current->getSuper()) {
        classDecl = dynamic_cast<ClassDecl*>(current->getRefNode());
        if (classDecl == 0) {
            continue;
        }
        if (strcmp(key, classDecl->GetName()) == 0 ||
            classDecl->ImplementsInterface(key)) {
            return true;
        }
    }*/
    return false;
}

bool SymbolTable::add(char* key, Node* node) {
    Symbol* s = new Symbol(VARIABLE, node);
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

int SymbolTable::getSize() {
    return table->NumEntries();
}
