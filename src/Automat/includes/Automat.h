/*
 * Automat.h
 *
 *      Author: gori1013
 */

#ifndef Automat_H_
#define Automat_H_
#include "../../Scanner/includes/TokenType.h"

class Automat {
public:
	Automat();
	virtual ~Automat();

	bool isFinalState(int state);
	int readChar(char c);
	int getCurrentState();
	int getFinalState();
    TokenType getTokenType(int state);
    bool shouldSaveChar();

private:
	void setNewState(int state);
	int getReturnChars(int state);
	int getTransitionColumn(char c);
	int getNextState(char c);
};

#endif /* Automat_H_ */
