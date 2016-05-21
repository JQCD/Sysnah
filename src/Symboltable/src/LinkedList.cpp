/*
 * ListElement.cpp
 *
 *      Author: gori1013
 */

#include "../includes/LinkedList.h"

LinkedList::LinkedList() {
	first = 0;
	last = 0;
}

LinkedList::~LinkedList() {
	delete first;
    // Ja, ich war doof :D
}

bool LinkedList::contains(const char* lexem) {
	return get(lexem) != 0;
}

Information* LinkedList::get(const char* lexem) {
	if (first == 0) {
		return 0;
	}

	if (last->equals(lexem)) {
		return last->getInformation();
	}

	ListElement* element = first;
	while (element != 0) {
		if (element->equals(lexem)) {
			return element->getInformation();
		}

		element = element->getNextElement();
	}

	return 0;
}

Information* LinkedList::insert(const char* lexem, TokenType tokenType) {
	Information* information = new Information(lexem, tokenType);
	ListElement* element = new ListElement(information);
	if (last == 0) {
		first = element;
		last = element;
	} else {
		last->setNextElement(element);
		last = element;
	}
	return information;
}
