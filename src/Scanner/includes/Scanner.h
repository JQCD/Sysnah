#ifndef _Scanner_H_
#define _Scanner_H_

#include "../../Buffer/includes/Buffer.h"
#include "../../Automat/includes/Automat.h"
#include "../../Symboltable/includes/Symboltable.h"
#include "Token.h"
#include "../../Symboltable/includes/Information.h"

class Scanner {
public:
	Scanner(const char *path, Symboltable *symtable);
	virtual ~Scanner();
	Token* nextToken();
private:
	Buffer *buffer;
	Automat *automat;
	Symboltable *symtable;
	Information parseIdentifier(char *lexem);
	Token *parseToken(int line, int column, int state);
	char *currTokenValue;
	char currentChar;
	int currTokenValueIndex;
    int maxLexemLength;
	bool eof;
};

#endif
