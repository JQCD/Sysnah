/*
 * Symboltable.cpp
 */

#include "../includes/Symboltable.h"
#include "stdio.h"

Symboltable::Symboltable() {
	// initialize key-value-lists as empty lists [K:Lexem][V:Information]
	for (int i = 0; i < SIZE; i++) {
		lists[i] = 0;
	}

	initSymbols();
}

Symboltable::~Symboltable() {
	for (int i = 0; i < SIZE; i++) {
		delete lists[i];
	}
}

// insert keywords into symtab
void Symboltable::initSymbols(){
	// define all keywords
	this->insert("IF", tokenIf);
	this->insert("if", tokenIf);
	this->insert("ELSE", tokenElse);
	this->insert("else", tokenElse);
	this->insert("WHILE", tokenWhile);
	this->insert("while", tokenWhile);
	this->insert("read", tokenRead);
	this->insert("write", tokenWrite);
	this->insert("int", tokenInt);
}

/** Insert lexem into the SymTab.
 *  If lexem already in SymTab -> return it's information,
 * 	otherwise add lexem and return it's information.
 */
Information* Symboltable::insert(const char* lexem, TokenType tokenType) {
	LinkedList* linkedList = getLinkedList(lexem);

	// check, if lexem is already in the SymTab
	Information* information = linkedList->get(lexem);
	if (information != 0) {
		return information;
	}

	// insert new lexem and return it's information
	return linkedList->insert(lexem, tokenType);
}

// get appropriate LinkedList (new/existing) for given lexem
LinkedList* Symboltable::getLinkedList(const char* lexem) {
	int listIndex = getLinkedListIndex(lexem);

	// initializes new LinkedList if there is no existing LinkedList
	if (this->lists[listIndex] == 0) {
		this->lists[listIndex] = new LinkedList();
	}

	// return existing/new list
	return this->lists[listIndex];
}

// get hash-value as appropriate LinkedListIndex for given lexem
int Symboltable::getLinkedListIndex(const char* lexem) {
	unsigned int h = hash(lexem);
	return h % SIZE;
}

// get hash-value for given lexem based on its' char-values.
unsigned int Symboltable::hash(const char* lexem) {
	unsigned int h = 0;

	for (int i = 0; lexem[i] != '\0'; i++) {
		h = 31*h + lexem[i];
	}
	return h;
}

bool Symboltable::contains(const char* lexem) {
	LinkedList* linkedList = getLinkedList(lexem);
	return linkedList->contains(lexem);
}

void Symboltable::storeIdentifierType(const char* identifier, unsigned int identifierType) {
	LinkedList* linkedList = getLinkedList(identifier);

	Information* information = linkedList->get(identifier);
	if (information == 0) {
		return; // TODO: ERROR
	}
	information->setIdentifierType(identifierType);
}

unsigned int Symboltable::getIdentifierType(const char* identifier) {
	LinkedList* linkedList = getLinkedList(identifier);

	Information* information = linkedList->get(identifier);
	if (information == 0) {
		return -1; // TODO: ERROR
	}
	return information->getIdentifierType();
}
