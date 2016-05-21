/*
 * ListElement.cpp
 *
 *      Author: gori1013
 */

#include "../includes/ListElement.h"


ListElement::ListElement(Information* information) {
	this->information = information;
	this->nextElement = 0;
}

ListElement::~ListElement() {
	delete nextElement;
    delete information;
}

Information* ListElement::getInformation() {
	return this->information;
}

bool ListElement::equals(const char* other) {
	return this->information->equals(other);
}

bool ListElement::isEmpty() {
	return this->information == 0;
}

void ListElement::setNextElement(ListElement* nextElement) {
	this->nextElement = nextElement;
}

ListElement* ListElement::getNextElement() {
	return nextElement;
}
