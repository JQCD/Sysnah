
/*
 * Scanner.cpp
 *
 * Created on: 09.04.2015
 *	   Author: tobias
 */

#include "../includes/Scanner.h"

#include <stdio.h>
#include <stdlib.h>

#define DEBUG false //if(DEBUG) fprintf(stderr, "Meldung");

 Scanner::Scanner(const char *path, Symboltable *symtable) {
	buffer = new Buffer(path);
	automat = new Automat();
	this->symtable = symtable;
	maxLexemLength = 2*buffer->getBuffersize();
	currTokenValue = new char[maxLexemLength];
	currTokenValueIndex = 0;
	currentChar = -1;
    this->eof = false;

	this->symtable->insert("IF", tokenIf);
	this->symtable->insert("if", tokenIf);
    this->symtable->insert("else", tokenElse);
    this->symtable->insert("ELSE", tokenElse);
	this->symtable->insert("WHILE", tokenWhile);
	this->symtable->insert("while", tokenWhile);
    this->symtable->insert("int", tokenInt);
    this->symtable->insert("read", tokenRead);
    this->symtable->insert("write", tokenWrite);
 }

 Scanner::~Scanner() {
	delete buffer;
	delete automat;
	delete[] currTokenValue;
 }

 Token* Scanner::nextToken() {
	if (currentChar == '\0' || this->eof) return new Token(NULL, -1, -1, tokenEof);
	int status, state;

    // skip whitespaces in front of tokens
    char c;
    do {
        c = buffer->getChar();
    } while (c == '\n' || c == '\r' || c == ' ' || c == '\t');
    buffer->ungetChar();


	int line = buffer->getLine();
	int column = buffer->getCol();

	Token *token = NULL;
	while(!token) {
		currentChar = buffer->getChar();
		if (currentChar < 0) {
			fprintf(stderr, "found out of bounds char near line %i, column %i. Check input file!\n"
						  , buffer->getLine(), buffer->getCol());
			exit(-1);
		}
		if (currentChar != '\0') {
			status = automat->readChar(currentChar);
		} else {
            this->eof = true;

            // force output of current token!
            state = automat->getFinalState();
            currTokenValue[currTokenValueIndex] = '\0';
            token = parseToken(line, column, state);
            if (token->getTokenType() == tokenUnknown) return new Token(NULL, line, column, tokenEof);
            return token;
		}

		// in final state
		if (status != 0) {
			if (status < 0) {
				// wrong char. return positive number of status in buffer and try again
				buffer->ungetChar((-status)+1);
			} else if (status > 0) {
				// status is last valid state.
				buffer->ungetChar();
			}
			// return one char to buffer and create token.
			state = automat->getFinalState();

			currTokenValue[currTokenValueIndex] = '\0';

			token = parseToken(line, column, state);

			currTokenValueIndex = 0;

		} else { // status == 0 nothing special, just a random valid state
			if (automat->shouldSaveChar()) {
				currTokenValue[currTokenValueIndex++] = currentChar;
				if (currTokenValueIndex >= this->maxLexemLength) {
					fprintf(stderr, "Maximum lexem-length reached!\n");
					exit(-1);
				}
			}
		}
	}
    if (DEBUG) fprintf(stderr, "Scanner returning %s at %i/%i\n", token->getTokenTypeStr(), token->getLine(), token->getCol());
	return token;
 }

 Token* Scanner::parseToken(int line, int column, int state) {
	Information *information = NULL;
	TokenType tokenType;

	tokenType = automat->getTokenType(state);
	switch (tokenType) {
	case tokenIdentifier:
		information = symtable->insert(currTokenValue, tokenIdentifier);
		tokenType = information->getTokenType();
		break;
	case tokenInteger:
		information = new Information(currTokenValue, tokenInteger);
		break;
	case tokenUnknown:
	default:
		information = new Information(currTokenValue, tokenUnknown);
	}
	return new Token(information, line, column, tokenType);
 }
