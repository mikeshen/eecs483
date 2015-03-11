#include "ast_decl.h"
#include "symbolTable.h"
/** Class: Symbol **/
Symbol::Symbol(E_Type t, Node* n) : type(t), node(n), env(nullptr) {}

Symbol::Symbol(E_Type t, Node* n, SymbolTable* e) : type(t), node(n), env(e) {}

/** Class: SymbolTable **/
SymbolTable::SymbolTable() : _parent(nullptr), _super(nullptr), _this(nullptr),
                             ownernode(nullptr), lastnode(nullptr),
                             table(new Hashtable<Symbol*>),
                             blocks(new List<SymbolTable*>) {}

Node* SymbolTable::getThisClass() {
    if (!_this) return nullptr;
    SymbolTable* temp = _this;
    return temp->getOwnerNode();
}

Symbol *SymbolTable::find(char* key) {
    SymbolTable* current = this;
    Symbol* symbol = nullptr;
    for ( ; current != nullptr; current = current->getParent())
        if ((symbol = current->findLocal(key)) != nullptr)
            return symbol;
    if (_super && (symbol = findSuper(key)) != nullptr) return symbol;
    return nullptr;
}

Symbol* SymbolTable::findLocal(char* key) {
    Symbol* symbol = nullptr;
    if ((symbol = table->Lookup(key)) != nullptr) return symbol;
    if (_super && (symbol = findSuper(key)) != nullptr) return symbol;
    return nullptr;
}

Symbol* SymbolTable::findInClass(char* key) {
    Symbol* symbol = nullptr;
    if ((symbol = findLocal(key)) != nullptr) return symbol;
    if ((symbol = findSuper(key)) != nullptr)  return symbol;
    return nullptr;
}

Symbol* SymbolTable::findUp(char* key) {
    SymbolTable* current = _parent;
    for ( ; current != nullptr; current = current->getParent()) {
        Symbol* symbol;
        if ((symbol = current->findLocal(key)) != nullptr)
            return symbol;
    }
    return nullptr;
}

Symbol* SymbolTable::findSuper(char *key) {
    SymbolTable* current = _super;
    if (!_super) return nullptr;
    for ( ; current != nullptr; current = current->getSuper()) {
        Symbol* symbol;
        if ((symbol = current->findLocal(key)) != nullptr)
            return symbol;
    }
    return nullptr;
}

Symbol* SymbolTable::findClassField(char* className, char* fieldName) {
    Symbol* classSymbol = find(className);
    if (classSymbol == nullptr) return nullptr;
    return classSymbol->getEnv()->find(fieldName);
}

bool SymbolTable::subclassOf(char *key) {
/*    SymbolTable* current = _super;
    ClassDecl* classDecl = nullptr;
    if (!_super) {
        return false;
    }
    for ( ; current != nullptr; current = current->getSuper()) {
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
    Symbol* refNode = nullptr;
    newEnv->setParent(this);
    if (_this)
        newEnv->setThis(_this);
    refNode = new Symbol(type, node, newEnv);
    table->Enter(key, refNode, false);
    newEnv->setOwnerNode(node);
    lastnode = nullptr;
    return newEnv;
}

int SymbolTable::getSize() {
    return table->NumEntries();
}
