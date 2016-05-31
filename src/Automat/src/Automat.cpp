/*
 * Automat.cpp
 *
 * This class represents the state-machine.
 * Each state represents a TokenType, and with it we can describe the accepted gramma.
 */

#include "../includes/Automat.h"
#include "stdio.h"

//// the states that describe the gramma/transitions
//static int STATES[27][22] = {
////  0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21
////	END	a-z	0-9	+	-	:	*	<	>	=	!	&	;	(	)	{	}	[	]	_rn	?
//	{1,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	0,	26,	tokenUnknown}, 		//0 Start/UNKNOWN
//	{1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenIdentifier}, 	//1	Lexem
//	{1,	0,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenInteger}, 		//2 Zahl
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenPlus}, 		//3 +
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenMinus}, 		//4 -
//	{1,	0,	0,	0,	0,	0,	20,	0,	0,	19,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenDivision}, 	//5 :
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenMultiply}, 	//6 *
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenLess}, 		//7 <
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenGreater}, 		//8 >
//	{1,	0,	0,	0,	0,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenEqual}, 		//9 =
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenInvert}, 		//10 !
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	25,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenUnknown}, 		//11 & (maybe &&)
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenSemicolon}, 	//12 ;
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenOpenRound}, 	//13 (
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCloseRound}, 	//14 )
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenOpenCurly}, 	//15 {
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCloseCurly}, 	//16 }
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenOpenSquare}, 	//17 [
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCloseSquare}, 	//18 ]
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenAssign}, 		//19 :=
//	{0,	20,	20,	20,	20,	20,	21,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	tokenComment},		//20 :* comment
//	{0,	20,	20,	20,	20,	22,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	tokenComment},		//21 :* * comment probably end
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenComment}, 		//22 :* *: comment end
//	{0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenUnknown}, 		//23 =:
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenNotEqual},  	//24 =:=
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenConcatenate}, 	//25 &&
//	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	tokenUnknown}		//26 UNKNOWN CHARACTER
//};

// the states that describe the gramma/transitions
static int STATES[27][22] = {
//  0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21
//	END	a-z	0-9	+	-	:	*	<	>	=	!	&	;	(	)	{	}	[	]	_rn	?
	{1,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	0,	26,	tokenUnknown}, 		//0 Start/UNKNOWN
	{1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenIdentifier}, 	//1	Lexem
	{1,	0,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenInteger}, 		//2 Zahl
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenPlus}, 		//3 +
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenMinus}, 		//4 -
	{1,	0,	0,	0,	0,	0,	20,	0,	0,	19,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenDivision}, 	//5 :
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenMultiply}, 	//6 *
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenLess}, 		//7 <
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenGreater}, 		//8 >
	{1,	0,	0,	0,	0,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenEqual}, 		//9 =
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenInvert}, 		//10 !
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	25,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenUnknown}, 		//11 & (maybe &&)
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenSemicolon}, 	//12 ;
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenOpenRound}, 	//13 (
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCloseRound}, 	//14 )
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenOpenCurly}, 	//15 {
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCloseCurly}, 	//16 }
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenOpenSquare}, 	//17 [
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCloseSquare}, 	//18 ]
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenAssign}, 		//19 :=
	{0,	20,	20,	20,	20,	20,	21,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	tokenComment},		//20 :* comment
	{0,	20,	20,	20,	20,	22,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	20,	tokenComment},		//21 :* * comment probably end
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenComment}, 		//22 :* *: comment end
	{0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenUnknown}, 		//23 =:
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenNotEqual},  	//24 =:=
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, 	tokenCondAnd}, 	    //25 &&
	{1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	tokenUnknown}		//26 UNKNOWN CHARACTER
};

int currentState;
int lastState;
int lastFinalState = 0;

// distance (in #chars) to last finale state
int lastFinalStateCounter = 0;

Automat::Automat() {
	lastState = 0;
	currentState = 0;
}

Automat::~Automat() {
}

// check if given state is final one
bool Automat::isFinalState(int state) {
	return STATES[state][0] == 1;
}

// current char should be saved
/**bool Automat::isInValidState()
{
	return currentState != 0;
}
*/

int Automat::getTransitionColumn(char c) {
	if (c == ' ' || c == '\r' || c == '\n') {
		return 19;
	} else if ((c >= 'a' && c <= 'z') | (c >= 'A' && c <= 'Z')) {
		return 1;
	} else if (c >= '0' && c <= '9') {
		return 2;
	} else if (c == '+') {
		return 3;
	} else if (c == '-') {
		return 4;
	} else if (c == ':') {
		return 5;
	} else if (c == '*') {
		return 6;
	} else if (c == '<') {
		return 7;
	} else if (c == '>') {
		return 8;
	} else if (c == '=') {
		return 9;
	} else if (c == '!') {
		return 10;
	} else if (c == '&') {
		return 11;
	} else if (c == ';') {
		return 12;
	} else if (c == '(') {
		return 13;
	} else if (c == ')') {
		return 14;
	} else if (c == '{') {
		return 15;
	} else if (c == '}') {
		return 16;
	} else if (c == '[') {
		return 17;
	} else if (c == ']') {
		return 18;
	}
	return 20;
}

// determines the next state for a given char
int Automat::getNextState(char c) {
	int column = getTransitionColumn(c);
	int nextState = STATES[currentState][column];
	return nextState;
}

// sets the new state
void Automat::setNewState(int state) {
	lastState = currentState;
	currentState = state;

	if (isFinalState(currentState)) {
		lastFinalState = currentState;
		lastFinalStateCounter = 0;
	}
}

// forces Automat to return the last final state and jump back to state 0.
// Needed at EOF.
int Automat::getFinalState() {
	int state = lastFinalState;

	//printf("currentState => %i", currentState);
//	if (currentState == 20 || currentState == 21)
//	{
//
//	}

	setNewState(0);
	return state;
}

int Automat::getCurrentState() {
	return currentState;
}

/**
 * return values:
 * > 0 value is last valid state. current char is invalid for this state
 * = 0 nothing special, just doing stuff right now
 * < 0 error. return positive number of this value and try again.
 */
int Automat::readChar(char c) {
	int state = 0;
	int nextState = getNextState(c);

	if (nextState != 0) {
		lastFinalStateCounter++;
	}

	if (nextState == 0) {
		if (isFinalState(currentState)) {
			state = currentState;
			currentState = 0;
		} else {
			state = -lastFinalStateCounter;
			currentState = lastFinalState;
		}
	} else {
		setNewState(nextState);
	}

	return state;
}

TokenType Automat::getTokenType(int state) {
	return (TokenType) STATES[state][21];
}

bool Automat::shouldSaveChar() {
	return currentState != 0;
}
