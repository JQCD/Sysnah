/*
 * ListElement.cpp
 */

#include "../includes/LinkedList.h"

LinkedList::LinkedList() {
	first = 0;
	last = 0;
}

LinkedList::~LinkedList() {
	delete first;
}

bool LinkedList::contains(const char* lexem) {
	return get(lexem) != 0;
}

Information* LinkedList::get(const char* lexem) {
	// list empty
	if (first == 0) {
		return 0;
	}

	// last lexem is equal
	if (last->equals(lexem)) {
		return last->getInformation();
	}

	// search lexem inbetween 0 and last
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
