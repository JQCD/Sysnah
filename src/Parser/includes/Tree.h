#ifndef _Tree_H_
#define _Tree_H_

#include "../../Symboltable/includes/Symboltable.h"

#include <fstream> // for std::ofstream

enum Type {
    noType = 0,
    intType,
    intArrayType,
    arrayType,
    errorType,
    opPlus,
    opMinus,
    opMult,
    opDiv,
    opLess,
    opGreater,
    opEqual,
    opNotEqual,
    opAnd
};

class IParseTree {
public:
    virtual void typeCheck(Symboltable *symtable)=0;
    virtual void makeCode(std::ofstream *code)=0;
    Type type;
    int line, col;
    void setPos(int line, int col){this->line = line; this->col = col;};
    int getLine(){return this->line;};
    int getCol(){return this->col;};
};

class IProg : public virtual IParseTree {};
class IDecls : public virtual IParseTree {};
class IDecl : public virtual IParseTree {};
class IArray : public virtual IParseTree {};
class IStatements : public virtual IParseTree {};
class IStatement : public virtual IParseTree {};
class IExp : public virtual IParseTree {};
class IExp2 : public virtual IParseTree {};
class IIndex : public virtual IParseTree {};
class IOp : public virtual IParseTree {};
class IOp_Exp : public virtual IParseTree {
public:
    virtual IOp *getOp(){return 0;};
};


class Prog_I : public IProg {
public:
    Prog_I();
    virtual ~Prog_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IDecls *node1, IStatements *node2);
private:
    IDecls *decls;
    IStatements *statements;
};

class Decls_I : public IDecls {
public:
    Decls_I();
    virtual ~Decls_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IDecl *node1, IDecls *node2);
private:
    IDecl *decl;
    IDecls *decls;
};

class Decls_II : public IDecls {
public:
    Decls_II();
    virtual ~Decls_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
private:

};

class Decl_I : public IDecl {
public:
    Decl_I(const char *identifier);
    virtual ~Decl_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IArray *array);
private:
    IArray *array;
    const char *identifier;
};

class Array_I : public IArray {
public:
    Array_I(long integer);
    virtual ~Array_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
private:
    long integer;
};

class Array_II : public IArray {
public:
    Array_II();
    virtual ~Array_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Statements_I : public IStatements {
public:
    Statements_I();
    virtual ~Statements_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IStatement *node1, IStatements *node2);
private:
    IStatement *statement;
    IStatements *statements;
};

class Statements_II : public IStatements {
public:
    Statements_II();
    virtual ~Statements_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Statement_I : public IStatement {
public:
    Statement_I(const char *identifier);
    virtual ~Statement_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IIndex *node1, IExp *node2);
private:
    const char *identifier;
    IIndex *index;
    IExp *exp;
};

class Statement_II : public IStatement {
public:
    Statement_II();
    virtual ~Statement_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp *node);
private:
    IExp *exp;
};

class Statement_III : public IStatement {
public:
    Statement_III(const char *identifier);
    virtual ~Statement_III();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IIndex *node);
private:
    const char *identifier;
    IIndex *index;
};

class Statement_IV : public IStatement {
public:
    Statement_IV();
    virtual ~Statement_IV();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IStatements *node);
private:
    IStatements *statements;
};

class Statement_V : public IStatement {
public:
    Statement_V();
    virtual ~Statement_V();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp *node1, IStatement *node2, IStatement *node3);
private:
    IExp *exp;
    IStatement *statement1;
    IStatement *statement2;
};

class Statement_VI : public IStatement {
public:
    Statement_VI();
    virtual ~Statement_VI();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp *node1, IStatement *node2);
private:
    IExp *exp;
    IStatement *statement;
};

class Exp_I : public IExp {
public:
    Exp_I();
    virtual ~Exp_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp2 *node1, IOp_Exp *node2);
private:
    IExp2 *exp2;
    IOp_Exp *op_exp;
};

class Exp2_I : public IExp2 {
public:
    Exp2_I();
    virtual ~Exp2_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp *node);
private:
    IExp *exp;
};

class Exp2_II : public IExp2 {
public:
    Exp2_II(const char *identifier);
    virtual ~Exp2_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IIndex *node);
private:
    const char *identifier;
    IIndex *index;
};

class Exp2_III : public IExp2 {
public:
    Exp2_III(long integer);
    virtual ~Exp2_III();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
private:
    long integer;
};

class Exp2_IV : public IExp2 {
public:
    Exp2_IV();
    virtual ~Exp2_IV();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp2 *node);
private:
    IExp2 *exp2;
};

class Exp2_V : public IExp2 {
public:
    Exp2_V();
    virtual ~Exp2_V();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp2 *node);
private:
    IExp2 *exp2;
};

class Index_I : public IIndex {
public:
    Index_I();
    virtual ~Index_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IExp *node);
private:
    IExp *exp;
};

class Index_II : public IIndex {
public:
    Index_II();
    virtual ~Index_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_Exp_I : public IOp_Exp {
public:
    Op_Exp_I();
    virtual ~Op_Exp_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
    void addNodes(IOp *node1, IExp *node2);
    IOp *getOp();
private:
    IOp *op;
    IExp *exp;
};

class Op_Exp_II : public IOp_Exp {
public:
    Op_Exp_II();
    virtual ~Op_Exp_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_I : public IOp {
public:
    Op_I();
    virtual ~Op_I();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_II : public IOp {
public:
    Op_II();
    virtual ~Op_II();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_III : public IOp {
public:
    Op_III();
    virtual ~Op_III();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_IV : public IOp {
public:
    Op_IV();
    virtual ~Op_IV();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_V : public IOp {
public:
    Op_V();
    virtual ~Op_V();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_VI : public IOp {
public:
    Op_VI();
    virtual ~Op_VI();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_VII : public IOp {
public:
    Op_VII();
    virtual ~Op_VII();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_VIII : public IOp {
public:
    Op_VIII();
    virtual ~Op_VIII();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

class Op_IX : public IOp {
public:
    Op_IX();
    virtual ~Op_IX();
    void typeCheck(Symboltable *symtable);
    void makeCode(std::ofstream *code);
};

#endif
