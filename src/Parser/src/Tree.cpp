

#define ENDL "\n";
#define ERROR(x, line, col) fprintf(stderr, "%s at line %i, column %i\n", x, line, col);// exit(-1); //Soll das Programm hier beenden?

#include "../includes/Tree.h"
#include "../includes/Parser.h"
#include "../../Symboltable/includes/Symboltable.h"

#include <stdlib.h>

#define DEBUG false

// DECLS STATEMENTS
Prog_I::Prog_I() {}
Prog_I::~Prog_I() {}

void Prog_I::typeCheck(Symboltable *symtable) {
    this->decls->typeCheck(symtable);
    this->statements->typeCheck(symtable);
    this->type = noType;
}

void Prog_I::makeCode(std::ofstream *code) {
    this->decls->makeCode(code);
    this->statements->makeCode(code);
    *code << "STP" << ENDL;
}

void Prog_I::addNodes(IDecls *node1, IStatements *node2) {
    this->decls = node1;
    this->statements = node2;
}

// DECL ; DECLS
Decls_I::Decls_I() {}
Decls_I::~Decls_I() {}

void Decls_I::makeCode(std::ofstream *code) {
    this->decl->makeCode(code);
    this->decls->makeCode(code);
}

void Decls_I::typeCheck(Symboltable *symtable) {
    this->decl->typeCheck(symtable);
    this->decls->typeCheck(symtable);
    this->type = noType;
}

void Decls_I::addNodes(IDecl *node1, IDecls *node2) {
    this->decl = node1;
    this->decls = node2;
}

// epsilon
Decls_II::Decls_II() {}
Decls_II::~Decls_II() {}

void Decls_II::makeCode(std::ofstream *code) {}

void Decls_II::typeCheck(Symboltable *symtable) {
    this->type = noType;
}

// int ARRAY identifier
Decl_I::Decl_I(const char *identifier) { this->identifier = identifier; }
Decl_I::~Decl_I() {}

void Decl_I::makeCode(std::ofstream *code) {
    *code << "DS " << "$" << this->identifier << " ";
    this->array->makeCode(code);
}

void Decl_I::typeCheck(Symboltable *symtable) {
    this->array->typeCheck(symtable);
    if (symtable->getIdentifierType(this->identifier) != noType) {
        if (DEBUG) fprintf(stderr, "%s is %i\n", this->identifier, symtable->getIdentifierType(this->identifier));
        this->type = errorType;
        ERROR("identifier already defined", this->getLine(), this->getCol());
    } else if (this->array->type == errorType) {
        this->type = errorType;
    } else {
        this->type = noType;
        if (this->array->type == arrayType) symtable->storeIdentifierType(this->identifier, intArrayType);
        else symtable->storeIdentifierType(this->identifier, intType);
    }
}

void Decl_I::addNodes(IArray *array) {
    this->array = array;
}

// [ integer ]
Array_I::Array_I(long integer) { this->integer = integer; }
Array_I::~Array_I() {}

void Array_I::makeCode(std::ofstream *code) {
    *code << this->integer << ENDL;
}

void Array_I::typeCheck(Symboltable *symtable) {
    if (this->integer > 0) this->type = arrayType;
    else {
        this->type = errorType;
        ERROR("no valid dimension", this->getLine(), this->getCol());
    }
}

// epsilon
Array_II::Array_II() {}
Array_II::~Array_II() {}

void Array_II::makeCode(std::ofstream *code) {
    *code << 1 << ENDL;
}

void Array_II::typeCheck(Symboltable *symtable) {
    this->type = noType;
}

// STATEMENT ; STATEMENTS
Statements_I::Statements_I() {}
Statements_I::~Statements_I() {}

void Statements_I::typeCheck(Symboltable *symtable) {
    this->statement->typeCheck(symtable);
    this->statements->typeCheck(symtable);
    this->type = noType;
}

void Statements_I::makeCode(std::ofstream *code) {
    this->statement->makeCode(code);
    this->statements->makeCode(code);
}

void Statements_I::addNodes(IStatement *node1, IStatements *node2) {
    this->statement = node1;
    this->statements = node2;
}

// epsilon
Statements_II::Statements_II() {}
Statements_II::~Statements_II() {}

void Statements_II::typeCheck(Symboltable *symtable) {
    this->type = noType;
}

void Statements_II::makeCode(std::ofstream *code) {
    *code << "NOP" << ENDL;
}

// identifier INDEX := EXP
Statement_I::Statement_I(const char *identifier) { this->identifier = identifier; }
Statement_I::~Statement_I() {}

void Statement_I::typeCheck(Symboltable *symtable) {
    this->exp->typeCheck(symtable);
    this->index->typeCheck(symtable);
    if (symtable->getIdentifierType(this->identifier) == noType) { // GET
        ERROR("identifier not defined", this->getLine(), this->getCol());
        this->type = errorType;
    } else if (this->exp->type == intType
        && ((symtable->getIdentifierType(this->identifier) == intType && this->index->type == noType)
        || (symtable->getIdentifierType(this->identifier) == intArrayType && this->index->type == arrayType))) this->type = noType;
    else {
        this->type = errorType;
        ERROR("incompatible types", this->getLine(), this->getCol());
    }
}

void Statement_I::makeCode(std::ofstream *code) {
    this->exp->makeCode(code);
    *code << "LA " << "$" << this->identifier << ENDL;
    this->index->makeCode(code);
    *code << "STR" << ENDL;
}

void Statement_I::addNodes(IIndex *node1, IExp *node2) {
    this->index = node1;
    this->exp = node2;
}

// write( EXP )
Statement_II::Statement_II() {}
Statement_II::~Statement_II() {}

void Statement_II::typeCheck(Symboltable *symtable) {
    this->exp->typeCheck(symtable);
    this->type = noType;
}

void Statement_II::makeCode(std::ofstream *code) {
    this->exp->makeCode(code);
    *code << "PRI" << ENDL;
}

void Statement_II::addNodes(IExp *node) {
    this->exp = node;
}

// read ( identifier INDEX)
Statement_III::Statement_III(const char *identifier) { this->identifier = identifier; }
Statement_III::~Statement_III() {}

void Statement_III::typeCheck(Symboltable *symtable) {
    this->index->typeCheck(symtable);
    if (symtable->getIdentifierType(this->identifier) == noType) {
        this->type = errorType;
        ERROR("identifier not defined", this->getLine(), this->getCol());
    } else if (((symtable->getIdentifierType(this->identifier) == intType) && this->index->type == noType)
        || ((symtable->getIdentifierType(this->identifier) == intArrayType) && this->index->type == arrayType)) {
            this->type = noType;
        }
    else {
        this->type = errorType;
        ERROR("incompatible types", this->getLine(), this->getCol());
    }
}

void Statement_III::makeCode(std::ofstream *code) {
    *code << "REA" << ENDL;
    *code << "LA " << "$" << this->identifier << ENDL;
    this->index->makeCode(code);
    *code << "STR" << ENDL;
}

void Statement_III::addNodes(IIndex *node) {
    this->index = node;
}

// {STATEMENTS}
Statement_IV::Statement_IV() {}
Statement_IV::~Statement_IV() {}

void Statement_IV::typeCheck(Symboltable *symtable) {
    this->statements->typeCheck(symtable);
    this->type = noType;
}

void Statement_IV::makeCode(std::ofstream *code) {
    this->statements->makeCode(code);
}

void Statement_IV::addNodes(IStatements *node) {
    this->statements = node;
}

// if ( EXP ) STATEMENT else STATEMENT
Statement_V::Statement_V() {}
Statement_V::~Statement_V() {}

void Statement_V::typeCheck(Symboltable *symtable) {
    this->exp->typeCheck(symtable);
    this->statement1->typeCheck(symtable);
    this->statement2->typeCheck(symtable);
    if (this->exp->type == errorType) this->type = errorType;
    else this->type = noType;
}

void Statement_V::makeCode(std::ofstream *code) {
    unsigned int l1Id = Parser::nextLabelId();
    unsigned int l2Id = Parser::nextLabelId();
    this->exp->makeCode(code);
    *code << "JIN " << "#L" << l1Id << ENDL;
    this->statement1->makeCode(code);
    *code << "JMP " << "#L" << l2Id << ENDL;
    *code << "#L" << l1Id << " NOP" << ENDL;
    this->statement2->makeCode(code);
    *code << "#L" << l2Id << " NOP" << ENDL;
}

void Statement_V::addNodes(IExp *node1, IStatement *node2, IStatement *node3) {
    this->exp = node1;
    this->statement1 = node2;
    this->statement2 = node3;
}

// while ( EXP ) STATEMENT
Statement_VI::Statement_VI() {}
Statement_VI::~Statement_VI() {}

void Statement_VI::typeCheck(Symboltable *symtable) {
    this->exp->typeCheck(symtable);
    this->statement->typeCheck(symtable);
}

void Statement_VI::makeCode(std::ofstream *code) {
    unsigned int l1Id = Parser::nextLabelId();
    unsigned int l2Id = Parser::nextLabelId();
    *code << "#L" << l1Id << " NOP" << ENDL; // TODO LABEL
    this->exp->makeCode(code);
    *code << "JIN " << "#L" << l2Id << ENDL;
    this->statement->makeCode(code);
    *code << "JMP " << "#L" << l1Id << ENDL;
    *code << "#L" << l2Id << " NOP" << ENDL;
}

void Statement_VI::addNodes(IExp *node1, IStatement *node2) {
    this->exp = node1;
    this->statement = node2;
}

// EXP2 OP_EXP
Exp_I::Exp_I() {}
Exp_I::~Exp_I() {}

void Exp_I::typeCheck(Symboltable *symtable) {
    this->exp2->typeCheck(symtable);
    this->op_exp->typeCheck(symtable);
    if (this->op_exp->type == noType) this->type = this->exp2->type;
    else if (this->exp2->type != this->op_exp->type) this->type = errorType;
    else this->type = this->exp2->type;
}

void Exp_I::makeCode(std::ofstream *code) {
    if (this->op_exp->type == noType) this->exp2->makeCode(code);
    else if (this->op_exp->getOp()->type == opGreater) {
        this->op_exp->makeCode(code);
        this->exp2->makeCode(code);
        *code << "LES" << ENDL;
    } else if (this->op_exp->getOp()->type == opNotEqual) {
        this->exp2->makeCode(code);
        this->op_exp->makeCode(code);
        *code << "NOT" << ENDL;
    } else {
        this->exp2->makeCode(code);
        this->op_exp->makeCode(code);
    }
}

void Exp_I::addNodes(IExp2 *node1, IOp_Exp *node2) {
    this->exp2 = node1;
    this->op_exp = node2;
}

// ( EXP )
Exp2_I::Exp2_I() {}
Exp2_I::~Exp2_I() {}

void Exp2_I::typeCheck(Symboltable *symtable) {
    this->exp->typeCheck(symtable);
    this->type = this->exp->type;
}

void Exp2_I::makeCode(std::ofstream *code) {
    this->exp->makeCode(code);
}

void Exp2_I::addNodes(IExp *node) {
    this->exp = node;
}

// identifier INDEX
Exp2_II::Exp2_II(const char *identifier) { this->identifier = identifier; }
Exp2_II::~Exp2_II() {}

void Exp2_II::typeCheck(Symboltable *symtable) {
    this->index->typeCheck(symtable);
    if (symtable->getIdentifierType(this->identifier) == noType) {
        this->type = errorType;
        ERROR("identifier not defined", this->getLine(), this->getCol());
    } else if (symtable->getIdentifierType(this->identifier) == intType && this->index->type == noType) this->type = (Type) symtable->getIdentifierType(this->identifier);
    else if (symtable->getIdentifierType(this->identifier) == intArrayType && this->index->type == arrayType) this->type = intType;
    else {
        this->type = errorType;
        ERROR("no primitive type", this->getLine(), this->getCol());
    }
}

void Exp2_II::makeCode(std::ofstream *code) {
    *code << "LA " << "$" << this->identifier << ENDL;
    this->index->makeCode(code);
    *code << "LV" << ENDL;
}

void Exp2_II::addNodes(IIndex *node) {
    this->index = node;
}

// integer
Exp2_III::Exp2_III(long integer) { this->integer = integer; }
Exp2_III::~Exp2_III() {}

void Exp2_III::typeCheck(Symboltable *symtable) {
    this->type = intType;
}

void Exp2_III::makeCode(std::ofstream *code) {
    *code << "LC " << this->integer << ENDL;
}

// - EXP2
Exp2_IV::Exp2_IV() {}
Exp2_IV::~Exp2_IV() {}

void Exp2_IV::typeCheck(Symboltable *symtable) {
    this->exp2->typeCheck(symtable);
    this->type = this->exp2->type;
}

void Exp2_IV::makeCode(std::ofstream *code) {
    *code << "LC " << "0" << ENDL;
    this->exp2->makeCode(code);
    *code << "SUB" << ENDL;
}

void Exp2_IV::addNodes(IExp2 *node) {
    this->exp2 = node;
}

// ! EXP2
Exp2_V::Exp2_V() {}
Exp2_V::~Exp2_V() {}

void Exp2_V::typeCheck(Symboltable *symtable) {
    this->exp2->typeCheck(symtable);
    if (this->exp2->type != intType) this->type = errorType;
    else this->type = intType;
}

void Exp2_V::makeCode(std::ofstream *code) {
    this->exp2->makeCode(code);
    *code << "NOT" << ENDL;
}

void Exp2_V::addNodes(IExp2 *node) {
    this->exp2 = node;
}

// [ EXP ]
Index_I::Index_I() {}
Index_I::~Index_I() {}

void Index_I::typeCheck(Symboltable *symtable) {
    this->exp->typeCheck(symtable);
    if (this->exp->type == errorType) this->type = errorType;
    else this->type = arrayType;
}

void Index_I::makeCode(std::ofstream *code) {
    this->exp->makeCode(code);
    *code << "ADD" << ENDL;
}

void Index_I::addNodes(IExp *node) {
    this->exp = node;
}

// epsilon
Index_II::Index_II() {}
Index_II::~Index_II() {}

void Index_II::typeCheck(Symboltable *symtable) {
    this->type = noType;
}

void Index_II::makeCode(std::ofstream *code) {}

// OP EXP
Op_Exp_I::Op_Exp_I() {}
Op_Exp_I::~Op_Exp_I() {}

void Op_Exp_I::typeCheck(Symboltable *symtable) {
    this->op->typeCheck(symtable);
    this->exp->typeCheck(symtable);
    this->type = this->exp->type;
}

void Op_Exp_I::makeCode(std::ofstream *code) {
    this->exp->makeCode(code);
    this->op->makeCode(code);
}

void Op_Exp_I::addNodes(IOp *node1, IExp *node2) {
    this->op = node1;
    this->exp = node2;
}

IOp* Op_Exp_I::getOp() {
    return this->op;
}

// epsilon
Op_Exp_II::Op_Exp_II() {}
Op_Exp_II::~Op_Exp_II() {}

void Op_Exp_II::typeCheck(Symboltable *symtable) {
    this->type = noType;
}

void Op_Exp_II::makeCode(std::ofstream *code) {}

// +
Op_I::Op_I() {}
Op_I::~Op_I() {}
void Op_I::typeCheck(Symboltable *symtable) { this->type = opPlus; }
void Op_I::makeCode(std::ofstream *code) { *code << "ADD" << ENDL; }

// -
Op_II::Op_II() {}
Op_II::~Op_II() {}
void Op_II::typeCheck(Symboltable *symtable) { this->type = opMinus; }
void Op_II::makeCode(std::ofstream *code) { *code << "SUB" << ENDL; }

// *
Op_III::Op_III() {}
Op_III::~Op_III() {}
void Op_III::typeCheck(Symboltable *symtable) { this->type = opMult; }
void Op_III::makeCode(std::ofstream *code) { *code << "MUL" << ENDL; }

// :
Op_IV::Op_IV() {}
Op_IV::~Op_IV() {}
void Op_IV::typeCheck(Symboltable *symtable) { this->type = opDiv; }
void Op_IV::makeCode(std::ofstream *code) { *code << "DIV" << ENDL; }

// <
Op_V::Op_V() {}
Op_V::~Op_V() {}
void Op_V::typeCheck(Symboltable *symtable) { this->type = opLess; }
void Op_V::makeCode(std::ofstream *code) { *code << "LES" << ENDL; }

// >
Op_VI::Op_VI() {}
Op_VI::~Op_VI() {}
void Op_VI::typeCheck(Symboltable *symtable) { this->type = opGreater; }
void Op_VI::makeCode(std::ofstream *code) {}

// =
Op_VII::Op_VII() {}
Op_VII::~Op_VII() {}
void Op_VII::typeCheck(Symboltable *symtable) { this->type = opEqual; }
void Op_VII::makeCode(std::ofstream *code) { *code << "EQU" << ENDL; }

// =:=
Op_VIII::Op_VIII() {}
Op_VIII::~Op_VIII() {}
void Op_VIII::typeCheck(Symboltable *symtable) { this->type = opNotEqual; }
void Op_VIII::makeCode(std::ofstream *code) { *code << "EQU" << ENDL; }

// &&
Op_IX::Op_IX() {}
Op_IX::~Op_IX() {}
void Op_IX::typeCheck(Symboltable *symtable) { this->type = opCondAnd; }
void Op_IX::makeCode(std::ofstream *code) { *code << "AND" << ENDL; }
