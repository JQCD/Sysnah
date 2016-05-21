/*
 * Information.cpp
 *
 *      Author: gori1013
 */

#include "../includes/Information.h"

#include <stdio.h> // for fprintf, snprintf, etc.
#include <stdlib.h> // for strtol
#include <errno.h> // for out-of-range detection


Information::Information(const char* lexem, TokenType tokenType) {

	this->lexem = copy(lexem);

	if (tokenType == tokenInteger) {
		long val = strtol(lexem, NULL, 10);
		if (errno == ERANGE) {
			fprintf(stderr, "Integer out of range!\n");
			exit(-2);
		}
		if (snprintf(this->lexem, getSize(lexem)+1, "%li", val) < 0) {
			fprintf(stderr, "Couldn't convert integer!\n");
			exit(-2);
		}
	}
		this->tokenType = tokenType;
		this->identifierType = 0;
}

Information::~Information() {
}

char* Information::getLexem() {
	return this->lexem;
}

bool Information::equals(const char* other) {
	int i = 0;
	while (lexem[i] != '\0' && lexem[i] == other[i]) {
		i++;
	}

	return lexem[i] == other[i];
}

TokenType Information::getTokenType() {
	return tokenType;
}

char* Information::copy(const char* other) {
	int size = getSize(other);

	this->lexem = new char[size + 1];

	for (int i = 0; i < size; i++) {
		lexem[i] = other[i];
	}

	lexem[size] = '\0';

	return lexem;
}

int Information::getSize(const char* lexem) {
	int size = 0;

	while (lexem[size] != '\0') {
		size++;
	}

	return size;
}

void Information::setIdentifierType(unsigned int identifierType) {
	this->identifierType = identifierType;
}

unsigned int Information::getIdentifierType() {
	return this->identifierType;
}

long Information::getInteger() {
	if (this->tokenType != tokenInteger) {
		return 0xDEADBEEF; //ERROR!, vllt. besser machen :D
	}
	return strtol(this->lexem, NULL, 10);
}
