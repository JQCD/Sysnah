/*
 * Scanner.cpp
 */

#include "../includes/Scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcmp(...)

#define DEBUG true //if(DEBUG) fprintf(stderr, "Meldung");

/**
 * Constructs a scanner.
 * It gets initiated with a path to the text-file and a symboltable.
 */
Scanner::Scanner(const char *path, Symboltable *symtable) {
	buffer = new Buffer(path);
	automat = new Automat();
	this->symtable = symtable;

	// set inital parameters like maximum length of lexems, current char or the EOF-flag
	maxStringLength = 2 * buffer->getBuffersize();
	currentString = new char[maxStringLength]; // create empty char-array with max length
	currentStringIndex = 0;
	currentChar = -1;
	this->eof = false;
}

Scanner::~Scanner() {
	delete buffer;
	delete automat;
	delete[] currentString;
}

// scan for next token
Token* Scanner::nextToken() {
	// check, if buffer reached EOF
	if (currentChar == '\0' || this->eof) {
		return new Token(NULL, -1, -1, tokenEof);
	}

	int status, state;
	char c;

	// 1. skip whitespaces until non-whitespace-char is found
	do {
		// read next char
		c = buffer->getChar();
	} while (c == '\n' || c == '\r' || c == ' ' || c == '\t');

	// 2. go back one char
	buffer->ungetChar();

	// 3. set starting-position and create NULL-token
	int line = buffer->getLine();
	int column = buffer->getCol();
	Token *token = NULL;

	while (!token) {
		// get next char
		currentChar = buffer->getChar();

		if (currentChar < 0) {
			// unknown char e.g. 'ü'. It's not in the compilers range of readable char's.
			// terminate with error-message.
			fprintf(stderr,
					"Found illegal char near line %i, column %i. Check input file!\n",
					buffer->getLine(), buffer->getCol());
			//exit(-1);
		}

		if (currentChar != '\0') {
			// automat processes current character
			status = automat->readChar(currentChar);
		} else {
			// EOF is reached
			this->eof = true;

			// get last final token
			state = automat->getFinalState();
			currentString[currentStringIndex] = '\0';
			token = parseToken(line, column, state);

			// create EOF-Token and return it
			if (token->getTokenType() == tokenUnknown) {
				return new Token(NULL, line, column, tokenEof);
			}

			return token;
		}

		if (status != 0) {
			// automat is in final state
			if (status < 0) {
				// invalid input. set buffer back to last final state
				buffer->ungetChar((-status) + 1);
			} else if (status > 0) {
				// status is last valid state
				buffer->ungetChar();
			}
			// automat returns to last final state
			state = automat->getFinalState();

			// close current string/token with EOF
			currentString[currentStringIndex] = '\0';

			// create token
			token = parseToken(line, column, state);

			// reset currentStringIndex (actually deletes last buffered string)
			currentStringIndex = 0;

		} else {
			// status is 0, so automat is in some valid state
			if (automat->shouldSaveChar() /** method is called _isInValidState()_ now */) {
				// add current char to the scanned string.
				currentString[currentStringIndex++] = currentChar;
				if (currentStringIndex >= this->maxStringLength) {
					// scanned string is too long. terminate with error-message
					fprintf(stderr,
							"Maximum string-length reached. Check input file!\n");
					exit(-1);
				}
			}
		}
	}

	if (DEBUG) {
		// print token-type of currently scanned string
		fprintf(stderr, "Scanner returning %s at %i/%i\n",
				token->getTokenTypeStr(), token->getLine(), token->getCol());
	}
	
	return token;
}

// create token
Token* Scanner::parseToken(int line, int column, int state) {
	Information *information = NULL;

	// get current token-type
	TokenType tokenType = automat->getTokenType(state);

	// create token-information
	switch (tokenType) {
		case tokenIdentifier:
			information = symtable->insert(currentString, tokenIdentifier);
			tokenType = information->getTokenType();
			break;
		case tokenInteger:
			information = new Information(currentString, tokenInteger);
			break;
		case tokenUnknown: // do default
		default:
			information = new Information(currentString, tokenUnknown);
	}

	// return parsed token
	return new Token(information, line, column, tokenType);
}

//void print_usage(bool toStdOut = false) {
//    if (toStdOut) printf("USAGE: ./scanner SOURCEFILE\n");
//    else fprintf(stderr, "USAGE: ./scanner SOURCEFILE\n");
//}
//
//// MAIN
//int main(int argc, char *argv[]) {
//    if (argc != 2) {
//        print_usage(false);
//        exit(-1);
//    }
//
//    char *source = argv[1];
//
//    if (source) {
//        Symboltable *symtable = new Symboltable();
//        Scanner *s = new Scanner(source, symtable);
//        Token* curToken = s->nextToken();
//
//        while (strcmp(curToken->getTokenTypeStr(), "Token EOF") != 0)
//        {
//        	if (DEBUG) {
//            	if ((strcmp(curToken->getTokenTypeStr(), "Token Identifier") == 0)
//            			|| (strcmp(curToken->getTokenTypeStr(), "Token Unknown") == 0) )
//            	{
//            		printf("Type:%-25s     Line:%-5i       Column:%-5i Lexem:%-20s\n",
//            			curToken->getTokenTypeStr(),
//    					curToken->getLine(), curToken->getCol(),curToken->getInformation()->getLexem());
//            	}
//            	else if (strcmp(curToken->getTokenTypeStr(), "Token Integer") == 0)
//            	{
//            		printf("Type:%-25s     Line:%-5i       Column:%-5i Value:%-20s\n",
//            			curToken->getTokenTypeStr(), curToken->getLine(), curToken->getCol(),
//    					curToken->getInformation()->getLexem());
//            	}
//            	else
//            	{
//               		printf("Type:%-25s     Line:%-5i       Column:%-5i\n",
//               			curToken->getTokenTypeStr(), curToken->getLine(), curToken->getCol());
//            	}
//        	}
//        	curToken = s->nextToken();
//        }
//        delete s;
//    } else {
//        fprintf(stderr, "Could not parse input and output files. Try again!\n");
//        exit(-3);
//    }
//}
