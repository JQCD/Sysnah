#ifndef _Parser_H_
#define _Parser_H_

#include "../../Scanner/includes/Scanner.h"
#include "../../Scanner/includes/Token.h"
#include "../../Symboltable/includes/Symboltable.h"
#include "Tree.h"

class Parser {

public:
    Parser(const char *path);
    virtual ~Parser();
    void parse(const char *path);

    static unsigned int nextLabelId();
private:
    void nextToken();

    Scanner *scanner;
    Token *currentToken;
    Symboltable *symtable;

    IProg* PROG();
    IDecls* DECLS();
    IDecl* DECL();
    IArray* ARRAY();
    IStatements* STATEMENTS();
    IStatement* STATEMENT();
    IExp* EXP();
    IExp2* EXP2();
    IIndex* INDEX();
    IOp_Exp* OP_EXP();
    IOp* OP();
};

#endif
