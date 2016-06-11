#include "../includes/Parser.h"
#include "../includes/Tree.h"
#include "../../Symboltable/includes/Symboltable.h"

#include <stdlib.h>
#include <stdio.h> // for std::ofstream, fprintf, etc
#include <string.h> // for strcmp(...)

#define currTokenType currentToken->getTokenType()
#define setPos() setPos(currentToken->getLine(), currentToken->getCol())
#define DEBUG true // if(DEBUG) fprintf(stderr, "\n");

inline void error(const char *msg, Token *t) {
    fprintf(stderr, "%s at line %i, column %i (got %s)\n", msg, t->getLine(), t->getCol(), t->getTokenTypeStr());
    exit(-1);
}

Parser::Parser(const char *path) {
    this->symtable = new Symboltable();
    this->scanner = new Scanner(path, symtable);
}

Parser::~Parser() {
    delete this->symtable;
    delete this->scanner;
}

void Parser::nextToken() {
    this->currentToken = this->scanner->nextToken();
    if (this->currentToken->getTokenType() == tokenEof) return;
    else {
        if (this->currentToken->getTokenType() == tokenComment) nextToken();
    }
}

unsigned int Parser::nextLabelId() {
    static unsigned int labelcount = 0;
    return labelcount++;
}

void Parser::parse(const char *path) {
    std::ofstream code(path, std::ios::out | std::ios::trunc);

    // Get first token to start parsing
    this->nextToken();

    printf("parsing...\n");
    IParseTree *parseTree = PROG();
    printf("type checking...\n");
    parseTree->typeCheck(this->symtable);
    printf("generating code...\n");
    parseTree->makeCode(&code);
    printf("stop.\n");

    code.close();
}

IProg* Parser::PROG() {
    if(DEBUG) fprintf(stderr, "Start PROG\n");

    // initialize tree
    IProg *tree = new Prog_I();

    // set current line and column
    tree->setPos();

    switch (currTokenType) {
    	// valid token-types
        case tokenInt:
        case tokenIdentifier:
        case tokenWrite:
        case tokenRead:
        case tokenOpenCurly:
        case tokenIf:
        case tokenWhile:
        case tokenEof: {
            if(DEBUG) fprintf(stderr, "Valid Token PROG_I, calling DECLS\n");
            IDecls *decls = DECLS();
            if(DEBUG) fprintf(stderr, "In PROG, calling STATEMENTS\n");
            static_cast<Prog_I*>(tree)->addNodes(decls, STATEMENTS());
            break;
        }

        default:
            error("unidentified token", currentToken);
    }

    return tree;
}

IDecls* Parser::DECLS() {
    IDecls *tree;
    switch (currTokenType) {
        case tokenInt: {
            if(DEBUG) fprintf(stderr, "Valid Token DECLS_I, calling DECL\n");
            tree = new Decls_I();
            tree->setPos();
            IDecl *decl = DECL();
            decl->setPos();
            if(DEBUG) fprintf(stderr, "In DECLS, next should be semicolon\n");
            if (currentToken->getTokenType() == tokenSemicolon) {
                nextToken();
                if(DEBUG) fprintf(stderr, "In DECLS, next was semicolon, calling DECLS\n");
                static_cast<Decls_I*>(tree)->addNodes(decl, DECLS());
            } else {
                error("Invalid Token", currentToken);
            }
            break;
        }
        case tokenIdentifier:
        case tokenWrite:
        case tokenRead:
        case tokenOpenCurly:
        case tokenIf:
        case tokenWhile:
        case tokenEof: {
            if(DEBUG) fprintf(stderr, "Valid Token DECLS_II\n");
            tree = new Decls_II();
            tree->setPos();
            break;
        }
        default:
            error("unidentified token", currentToken);
    }
    return tree;
}

IDecl* Parser::DECL() {
    IDecl *tree;
    if (currentToken->getTokenType() == tokenInt) {
        if(DEBUG) fprintf(stderr, "In DECL, valid TokenInt\n");
        nextToken();
        if(DEBUG) fprintf(stderr, "In DECL, calling ARRAY\n");
        IArray *array = ARRAY();
        if (currentToken->getTokenType() == tokenIdentifier) {
            tree = new Decl_I(currentToken->getInformation()->getLexem());
            tree->setPos();
            static_cast<Decl_I*>(tree)->addNodes(array);
            nextToken();
        } else {
            error("expected identifier", currentToken);
        }
    } else {
        error("unidentified token", currentToken);
    }
    return tree;
}

IArray* Parser::ARRAY() {
    IArray *tree;
    if (currentToken->getTokenType() == tokenOpenSquare) {
        nextToken();
        if (currentToken->getTokenType() == tokenInteger) {
            if(DEBUG) fprintf(stderr, "In ARRAY, valid Integer ahead (ARRAY_I)\n");
            tree = new Array_I(currentToken->getInformation()->getInteger());
            tree->setPos();
            nextToken();
            if (currentToken->getTokenType() == tokenCloseSquare) {
                nextToken();
            } else {
                error("expected closing bracket", currentToken);
            }
        } else {
            error("expected array dimension", currentToken);
        }
    } else if (currentToken->getTokenType() == tokenIdentifier) {
        if(DEBUG) fprintf(stderr, "Valid ARRAY_II\n");
        tree = new Array_II();
        tree->setPos();
    } else {
        error("unidentified token", currentToken);
    }
    return tree;
}

IStatements* Parser::STATEMENTS() {
    IStatements *tree;
    switch (currTokenType) {
        case tokenIdentifier:
        case tokenWrite:
        case tokenRead:
        case tokenOpenCurly:
        case tokenIf:
        case tokenWhile: {
            tree = new Statements_I();
            tree->setPos();
            IStatement *statement = STATEMENT();
            if (currentToken->getTokenType() == tokenSemicolon) {
                nextToken();
                if(DEBUG) fprintf(stderr, "In STATEMENTS, calling STATEMENTS\n");
                static_cast<Statements_I*>(tree)->addNodes(statement, STATEMENTS());
            } else error("Invalid Token", currentToken);
            break;
        }
        case tokenCloseCurly:
        case tokenEof: {
            if(DEBUG) fprintf(stderr, "Having STATEMENTS_II\n");
            tree = new Statements_II();
            tree->setPos();
            break;
        }
        default:
            error("unknown token", currentToken);
    }
    return tree;
}

IStatement* Parser::STATEMENT() {
    IStatement *tree;
    if(DEBUG) fprintf(stderr, "In STATEMENT\n");
    switch (currentToken->getTokenType()) {
        case tokenIdentifier: {
            if(DEBUG) fprintf(stderr, "Having Statement_I\n");
            tree = new Statement_I(currentToken->getInformation()->getLexem());
            tree->setPos();
            nextToken();
            if(DEBUG) fprintf(stderr, "Having Statement_I, calling INDEX\n");
            IIndex *index = INDEX();
            if (currentToken->getTokenType() != tokenAssign) error("expected assignment", currentToken);
            else {
                nextToken();
                if(DEBUG) fprintf(stderr, "Having Statement_I, calling EXP\n");
                static_cast<Statement_I*>(tree)->addNodes(index, EXP());
            }
            break;
        }
        case tokenWrite: {
            tree = new Statement_II();
            tree->setPos();
            if(DEBUG) fprintf(stderr, "Having Statement_II\n");
            nextToken();
            if (currentToken->getTokenType() != tokenOpenRound) error("expected opening paranthesis", currentToken);
            else {
                nextToken();
                if(DEBUG) fprintf(stderr, "Having Statement_II, calling EXP\n");
                static_cast<Statement_II*>(tree)->addNodes(EXP());
                if (currentToken->getTokenType() != tokenCloseRound) error("expected closing paranthesis", currentToken);
                else nextToken();
            }
            break;
        }
        case tokenRead: {
            nextToken();
            if (currentToken->getTokenType() != tokenOpenRound) error("expected opening paranthesis", currentToken);
            else {
                nextToken();
                if (currentToken->getTokenType() != tokenIdentifier) error("expected identifier", currentToken);
                else {
                    tree = new Statement_III(currentToken->getInformation()->getLexem());
                    tree->setPos();
                    nextToken();
                    static_cast<Statement_III*>(tree)->addNodes(INDEX());
                    if (currentToken->getTokenType() != tokenCloseRound) error("expected closing paranthesis", currentToken);
                    else nextToken();
                }
            }
            break;
        }
        case tokenOpenCurly: {
            tree = new Statement_IV();
            tree->setPos();
            nextToken();
            static_cast<Statement_IV*>(tree)->addNodes(STATEMENTS());
            if (currentToken->getTokenType() != tokenCloseCurly) error("expected closing braces", currentToken);
            else nextToken();
            break;
        }
        case tokenIf: {
            tree = new Statement_V();
            tree->setPos();
            nextToken();
            if (currentToken->getTokenType() != tokenOpenRound) error("expected opening paranthesis", currentToken);
            if (DEBUG) fprintf(stderr, "currenttoken: %s\n", currentToken->getTokenTypeStr());
            nextToken();
            if (DEBUG) fprintf(stderr, "currenttoken: %s\n", currentToken->getTokenTypeStr());
            IExp *exp = EXP();
            if (currentToken->getTokenType() != tokenCloseRound) error("expected closing paranthesis", currentToken);
            nextToken();
            IStatement *statement1 = STATEMENT();
            if (currentToken->getTokenType() != tokenElse) error("expected else", currentToken);
            nextToken();
            static_cast<Statement_V*>(tree)->addNodes(exp, statement1, STATEMENT());
            break;
        }
        case tokenWhile: {
            tree = new Statement_VI();
            tree->setPos();
            nextToken();
            if (currentToken->getTokenType() != tokenOpenRound) error("expected opening paranthesis", currentToken);
            nextToken();
            IExp *exp = EXP();
            if (currentToken->getTokenType() != tokenCloseRound) error("expected closing paranthesis", currentToken);
            nextToken();
            static_cast<Statement_VI*>(tree)->addNodes(exp, STATEMENT());
            break;
        }
        default:
            error("unidentified token", currentToken);
    }
    return tree;
}

IExp* Parser::EXP() {
    if (currTokenType != tokenOpenRound && currTokenType != tokenIdentifier && currTokenType != tokenInteger &&
        currTokenType != tokenMinus && currTokenType != tokenInvert) error("unknwon token", currentToken);
    IExp *tree = new Exp_I();
    tree->setPos();
    if(DEBUG) fprintf(stderr, "In EXP, calling EXP2\n");
    IExp2 *exp2 = EXP2();
    if(DEBUG) fprintf(stderr, "In EXP, calling OP_EXP\n");
    static_cast<Exp_I*>(tree)->addNodes(exp2, OP_EXP());
    return tree;
}

IExp2* Parser::EXP2() {
    IExp2 *tree;
    switch (currTokenType) {
        case tokenOpenRound: {
            if(DEBUG) fprintf(stderr, "Having Exp2_I, calling EXP\n");
            tree = new Exp2_I();
            tree->setPos();
            nextToken();
            static_cast<Exp2_I*>(tree)->addNodes(EXP());
            if (currentToken->getTokenType() != tokenCloseRound) error("expected closing paranthesis", currentToken);
            else nextToken();
            break;
        }
        case tokenIdentifier: {
            if(DEBUG) fprintf(stderr, "Having Exp2_II, calling INDEX\n");
            tree = new Exp2_II(currentToken->getInformation()->getLexem());
            tree->setPos();
            nextToken();
            static_cast<Exp2_II*>(tree)->addNodes(INDEX());
            break;
        }
        case tokenInteger: {
            if(DEBUG) fprintf(stderr, "Having Exp2_III\n");
            tree = new Exp2_III(currentToken->getInformation()->getInteger());
            tree->setPos();
            nextToken();
            break;
        }
        case tokenMinus: {
            if(DEBUG) fprintf(stderr, "Having Exp2_IV, calling EXP2\n");
            tree = new Exp2_IV();
            tree->setPos();
            nextToken();
            static_cast<Exp2_IV*>(tree)->addNodes(EXP2());
            break;
        }
        case tokenInvert: {
            tree = new Exp2_V();
            tree->setPos();
            if(DEBUG) fprintf(stderr, "Having Exp2_V, calling EXP2\n");
            nextToken();
            static_cast<Exp2_V*>(tree)->addNodes(EXP2());
            break;
        }
        default:
            error("unidentified token", currentToken);
    }
    if(DEBUG) fprintf(stderr, "In EXP2\n");
    return tree;
}

IIndex* Parser::INDEX() {
    IIndex *tree;
    switch (currentToken->getTokenType()) {
    case tokenOpenSquare: {
        tree = new Index_I();
        tree->setPos();
        nextToken();
        static_cast<Index_I*>(tree)->addNodes(EXP());
        if (currentToken->getTokenType() != tokenCloseSquare) error("expected closing brackets", currentToken);
        nextToken();
        break;
    }
    case tokenAssign:
    case tokenPlus:
    case tokenCloseRound:
    case tokenElse:
    case tokenMinus:
    case tokenMultiply:
    case tokenDivision:
    case tokenLess:
    case tokenGreater:
    case tokenNotEqual:
    case tokenCondAnd:
    case tokenEqual:
    case tokenSemicolon:
    case tokenCloseSquare: {
        tree = new Index_II();
        tree->setPos();
        break;
    }
    default:
        error("unidentified token", currentToken);
    }
    return tree;
}

IOp_Exp* Parser::OP_EXP() {
    IOp_Exp *tree;
    switch (currTokenType) {
    case tokenPlus:
    case tokenMinus:
    case tokenMultiply:
    case tokenDivision:
    case tokenLess:
    case tokenGreater:
    case tokenEqual:
    case tokenNotEqual:
    case tokenCondAnd: {
        tree = new Op_Exp_I();
        tree->setPos();
        IOp *op = OP();
        static_cast<Op_Exp_I*>(tree)->addNodes(op, EXP());
        break;
    }
    case tokenElse:
    case tokenCloseRound:
    case tokenSemicolon:
    case tokenCloseSquare: {
        tree = new Op_Exp_II();
        tree->setPos();
        break;
    }
    default:
        error("unidentified token", currentToken);
    }
    return tree;
}

IOp* Parser::OP() {
    IOp *tree;
    switch (currTokenType) {
        case tokenPlus: {
            tree = new Op_I();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenMinus: {
            tree = new Op_II();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenMultiply: {
            tree = new Op_III();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenDivision: {
            tree = new Op_IV();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenLess: {
            tree = new Op_V();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenGreater: {
            tree = new Op_VI();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenEqual: {
            tree = new Op_VII();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenNotEqual: {
            tree = new Op_VIII();
            tree->setPos();
            nextToken();
            break;
        }
        case tokenCondAnd: {
            tree = new Op_IX();
            tree->setPos();
            nextToken();
            break;
        }
        default:
        error("expected operator", currentToken);
    }

    return tree;

}

void print_usage(bool toStdOut = false) {
    if (toStdOut) printf("USAGE: parser SOURCEFILE -c OUTPUTFILE\n");
    else fprintf(stderr, "USAGE: parser SOURCEFILE -c OUTPUTFILE\n");
}

// MAIN

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(false);
        exit(-1);
    }

    char *source = NULL;
    char *code = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            code = argv[++i];
            continue;
        }
        source = argv[i];
    }

    if (source && code) {
        Parser *p = new Parser(source);
        p->parse(code);
        delete p;
    } else {
        fprintf(stderr, "Could not parse input and output files. Try again!\n");
        exit(-3);
    }

}
