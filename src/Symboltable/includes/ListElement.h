/*
 * ListElement.h
 */

#ifndef LISTELEMENT_H_
#define LISTELEMENT_H_

#include "Information.h"

class ListElement {
	Information* information;
	ListElement* nextElement;

public:
	ListElement(Information* information);
	virtual ~ListElement();

	Information* getInformation();
	bool equals(const char* lexem);
	bool isEmpty();
	void setNextElement(ListElement* nextElement);
	ListElement* getNextElement();
};

#endif /* LINKEDLIST_H_ */
