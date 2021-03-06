/*
 * Symboltable.h
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "LinkedList.h"
#include "Information.h"

class Symboltable {
	static const int SIZE = 32;

	LinkedList* lists[SIZE];

public:
	Symboltable();
	virtual ~Symboltable();

	Information* insert(const char* lexem, TokenType tokenType);
	bool contains(const char* lexem);

	// added for typecheck
	void storeIdentifierType(const char* identifier, unsigned int identifierType);
	unsigned int getIdentifierType(const char* identifier);

private:
	LinkedList* getLinkedList(const char* lexem);

	void initSymbols();
	unsigned int hash(const char* lexem);
	int getLinkedListIndex(const char* lexem);

};

#endif /* SYMBOLTABLE_H_ */
