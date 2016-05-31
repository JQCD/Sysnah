#ifndef _TOKENTYPE_H_
#define _TOKENTYPE_H_

enum TokenType {

    tokenInteger,		// 0
    tokenIdentifier,	// 1
    tokenPlus,			// 2
    tokenMinus,			//
    tokenDivision,		// 4
    tokenMultiply,		//
    tokenLess,			// 6
    tokenGreater,		//
    tokenEqual,			// 8
    tokenAssign,		//
    tokenNotEqual,		// 10
    tokenInvert,		//
    tokenCondAnd,		// 12
    tokenSemicolon,		//
    tokenOpenRound,		// 14
    tokenCloseRound,	//
    tokenOpenCurly,		// 16
    tokenCloseCurly,	//
    tokenOpenSquare,	// 18
    tokenCloseSquare,	//
    tokenUnknown,		// 20
    tokenComment,		//
    tokenIf,			// 22
    tokenWhile,			//
    COUNT_TOKEN_TYPE,	// 24
    tokenEof,			//
    tokenWrite,			// 26
    tokenRead,          //
    tokenInt,           // 28
    tokenElse           //
	// + 3 Einträge für Großschreibung: IF, ELSE, WHILE
};

#endif
