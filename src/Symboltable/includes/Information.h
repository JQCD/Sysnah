/*
 * Information.h
 *
 *      Author: gori1013
 */

#ifndef INFORMATION_H_
#define INFORMATION_H_

#include "../../Scanner/includes/TokenType.h"

class Information {
	char* lexem;
	TokenType tokenType;

private:
	char* copy(const char* other);
	int getSize(const char* lexem);

	// needed for typecheck
	unsigned int identifierType;

public:
	Information(const char* lexem, TokenType tokenType);
	virtual ~Information();

	char* getLexem();
	long getInteger();
	
	bool equals(const char* other);
	TokenType getTokenType();

	// needed for typecheck
	void setIdentifierType(unsigned int identifierType);
	unsigned int getIdentifierType();
};

#endif /* INFORMATION_H_ */
