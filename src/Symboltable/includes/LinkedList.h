/*
 * LinkedList.h
 *
 *      Author: gori1013
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "ListElement.h"

class LinkedList {
private:
	ListElement* first;
	ListElement* last;

public:
	LinkedList();
	virtual ~LinkedList();

	Information* insert(const char* lexem, TokenType tokenType);

	bool contains(const char* lexem);

	Information* get(const char* lexem);

};

#endif /* LINKEDLIST_H_ */
