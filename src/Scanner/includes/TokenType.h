#ifndef _TOKENTYPE_H_
#define _TOKENTYPE_H_

enum TokenType {
    tokenIdentifier,	// 0
    tokenInteger,		//
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
    tokenConcatenate,	// 12
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
    tokenWrite,
    tokenRead,
    tokenInt,
    tokenElse
};

#endif
