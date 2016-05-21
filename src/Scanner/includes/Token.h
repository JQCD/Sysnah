#ifndef _Token_H_
#define _Token_H_

#include "../../Symboltable/includes/Information.h"

class Token {
public:
	Token(Information *information, int line, int col, TokenType tokentype);
	virtual ~Token();
    Information *getInformation();
    int getLine();
    int getCol();
    TokenType getTokenType();
    const char *getTokenTypeStr();
private:
	Information *information;
    int line;
    int col;
    TokenType tokentype;
};

#endif
