/* File: ast_type.h
 * ----------------
 * In our parse tree, Type nodes are used to represent and
 * store type information. The base Type class is used
 * for built-in types, the NamedType for classes and interfaces,
 * and the ArrayType for arrays of other types.
 *
 * pp3: You will need to extend the Type classes to implement
 * the type system and rules for type equivalency and compatibility.
 */

#ifndef _H_ast_type
#define _H_ast_type

#include "ast.h"
#include "list.h"
#include <iostream>


class Type : public Node
{
  protected:
	char* typeName;

  public :
	static Type* intType,* doubleType,* boolType,* voidType,
				*nullType,* stringType,* errorType;

	Type(yyltype loc) : Node(loc) {}
	Type(const char* str);

	virtual void PrintToStream(std::ostream& out) { out << typeName; }
	friend std::ostream& operator<<(std::ostream& out, Type* t) { t->PrintToStream(out); return out; }
	virtual bool IsEquivalentTo(Type* inputType) { return this == inputType; }
	virtual bool Check(SymbolTable* symT) { return true; }
	virtual bool isBuiltIn() { return true; }
	virtual bool isEquivalentTo(Type* inputType) { return this == inputType; }
	virtual bool isConvertableTo(Type* inputType) {
		if (this == inputType || this == errorType)
			return true;
		return !inputType->isBuiltIn() && this == nullType;
	}
	virtual char* getName() { return typeName; }
	virtual int getQualifier() { return 0; }
	virtual Identifier* getIdentifier() { return NULL; }
};

class NamedType : public Type
{
  protected:
	Identifier* id;

  public:
	NamedType(Identifier* i);
	virtual bool Check(SymbolTable* symT) { return symT->find(id->getName()) != NULL; }
	void PrintToStream(std::ostream& out) { out << id; }

	char* getName() { return id->getName();}
	Identifier* getIdentifier(){ return id; }
	bool isBuiltIn() { return false; }
	bool isEquivalentTo(Type* inputType);
	bool isConvertableTo(Type* inputType);
};

class ArrayType : public Type
{
  protected:
	Type* elemType;

  public:
	ArrayType(yyltype loc, Type* elemType);
	virtual bool Check(SymbolTable* symT) { return elemType->Check(symT); }
	void PrintToStream(std::ostream& out) { out << elemType << "[]"; }
	bool isBuiltIn() { return false; }
	bool isEquivalentTo(Type* inputType);
	bool isConvertableTo(Type* inputType);
	Type* getElemType() { return elemType; }
	Identifier* getIdentifier() { return elemType->getIdentifier(); }
};


#endif
