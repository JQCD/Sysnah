/*
 * Symboltable.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Symboltable.h"

Symboltable::Symboltable() {
	for (int i = 0; i < SIZE; i++) {
		lists[i] = 0;
	}
}

Symboltable::~Symboltable() {
	for (int i = 0; i < SIZE; i++) {
		delete lists[i];
	}
}

Information* Symboltable::insert(const char* lexem, TokenType tokenType) {
	LinkedList* linkedList = getLinkedList(lexem);

	Information* information = linkedList->get(lexem);
	if (information != 0) {
		return information;
	}

	return linkedList->insert(lexem, tokenType);
}

LinkedList* Symboltable::getLinkedList(const char* lexem) {
	int listIndex = getLinkedListIndex(lexem);

	if (this->lists[listIndex] == 0) {
		this->lists[listIndex] = new LinkedList();
	}

	return this->lists[listIndex];
}

int Symboltable::getLinkedListIndex(const char* lexem) {
	unsigned int h = hash(lexem);
	return h % SIZE;
}

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