
#include "../includes/Token.h"

Token::Token(Information *information, int line, int col, TokenType tokentype) {
    this->information = information;
    this->line = line;
    this->col = col;
    this->tokentype = tokentype;
}

Token::~Token() {
    delete information;
}

Information *Token::getInformation() {
    return this->information;
}

int Token::getLine() {
    return this->line;
}

int Token::getCol() {
    return this->col;
}

TokenType Token::getTokenType() {
    return this->tokentype;
}

const char *Token::getTokenTypeStr() {
    switch (this->tokentype) 
    {
        case tokenIdentifier:
            return "Token Identifier";
        case tokenInteger:
            return "Token Integer";
        case tokenPlus:
            return "Token Plus";
        case tokenMinus:
            return "Token Minus";
        case tokenDivision:
            return "Token Division";
        case tokenMultiply:
            return "Token Multiply";
        case tokenLess:
            return "Token Less";
        case tokenGreater:
            return "Token Greater";
        case tokenEqual:
            return "Token Equal";
        case tokenAssign:
            return "Token Assign";
        case tokenNotEqual:
            return "Token Not Eql";
        case tokenInvert:
            return "Token Invert";
        case tokenCondAnd:
            return "Token Conditional And";
        case tokenSemicolon:
            return "Token Semicolon";
        case tokenOpenRound:
            return "Token Left Paranthesis";
        case tokenCloseRound:
            return "Token Right Paranthesis";
        case tokenOpenCurly:
            return "Token Left Braces";
        case tokenCloseCurly:
            return "Token Right Braces";
        case tokenOpenSquare:
            return "Token Left Brackets";
        case tokenCloseSquare:
            return "Token Right Brackets";
        case tokenComment:
            return "Token Comment";
        case tokenIf:
            return "Token If";
        case tokenWhile:
            return "Token While";
        case tokenUnknown:
            return "Token Unknown";
        case tokenEof:
            return "Token EOF";
        case tokenElse:
            return "Token else";
        default:
            return "Undefined Token!!!";
    }
}
