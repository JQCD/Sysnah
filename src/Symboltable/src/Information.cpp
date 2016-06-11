/*
 * Information.cpp
 */

#include "../includes/Information.h"

#include <stdio.h> // for fprintf, snprintf, etc.
#include <stdlib.h> // for strtol
#include <errno.h> // for out-of-range detection


Information::Information(const char* lexem, TokenType tokenType) {
	copy(lexem);

	if (tokenType == tokenInteger) {
		// strtol -> string to long conversion!
		long val = strtol(lexem, NULL, 10);

		// check for integer-overflow (ERANGE is max range)
		if (errno == ERANGE) {
			fprintf(stderr, "Integer out of range!\n");
			exit(-2);
		}

		// check for conversion-failure. (Long-value has to fit in the lexem)
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
	// '/0' is Ascii Code for EOF
	while (lexem[i] != '\0' && lexem[i] == other[i]) {
		i++;
	}

	return lexem[i] == other[i];
}

TokenType Information::getTokenType() {
	return tokenType;
}

// copies chars of given char* into this class' lexem-char*
void Information::copy(const char* other) {
	int size = getSize(other);

	this->lexem = new char[size + 1];

	for (int i = 0; i < size; i++) {
		this->lexem[i] = other[i];
	}

	this->lexem[size] = '\0';
}

// gives back the size of the current lexem
int Information::getSize(const char* lexem) {
	int size = 0;

	while (lexem[size] != '\0') {
		size++;
	}

	return size;
}

// set identifierType
void Information::setIdentifierType(unsigned int identifierType) {
	this->identifierType = identifierType;
}

// get identifierType
unsigned int Information::getIdentifierType() {
	return this->identifierType;
}

long Information::getInteger() {
	if (this->tokenType != tokenInteger) {
		return 0xDEADBEEF; //ERROR!, vllt. besser machen :D
	}
	return strtol(this->lexem, NULL, 10);
}
