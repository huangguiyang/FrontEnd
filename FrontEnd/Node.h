//
//  Node.h
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Node__
#define __FrontEnd__Node__

class Word;
class Type;
class Token;

class Node {
private:
    static int labels;
    
protected:
    
    Node();
    
public:
    
    int newlabel();
    void emitlabel(int l);
    void emit(const char *s);
};

// Expr
//  op:     operator
//  type:   return type

class Expr : public Node {
    
public:
    Token   *op;
    Type    *type;
    
    Expr(Token *t, Type *p);
    
    virtual void    jumping(int t, int f);
    void    emitjumps(const char *test, int t, int f);
    virtual const char *toString();
    
    virtual Expr    *gen();
    virtual Expr    *reduce();
};

// Id Expression
//

class Id : public Expr {
    
public:
    int offset;
    
    Id(Word *w, Type *p, int b);
};

// Op Expressions
//
//  Arith:
//  Access:
//  Unary:

class Op : public Expr {
    
public:
    
    Op(Token *t, Type *p);
    
    Expr *reduce();
};

class Arith : public Op {
private:
    Expr *expr1;
    Expr *expr2;
    
public:
    
    Arith(Token *t, Expr *e1, Expr *e2);
    
    Expr *gen();
    
    const char *toString();
};

class Access : public Op {
    
public:
    Id      *array;
    Expr    *index;
    
    Access(Id *a, Expr *i, Type *p);
    
    Expr *gen();
    
    void jumping(int t, int f);
    
    const char *toString();
};

class Unary : public Op {
    
public:
    Expr    *expr;
    
    Unary(Token *t, Expr *e);
    
    Expr *gen();
    
    const char *toString();
};

// Temp Expression
//
//

class Temp : public Expr {
private:
    static int count;
    int number;
    
public:
    
    Temp(Type *p);
    
    const char *toString();
};

// Constant Expression
//
//

class Constant : public Expr {
public:
    
    Constant(Token *t, Type *p);
    Constant(int i);
    
    static Constant *True;
    static Constant *False;
    
    void jumping(int t, int f);
};

//  Logical Expressions
//
//  Or:     ||
//  And:    &&
//  Not:    !
//  Rel:    >, >=, <, <=, ==, !=

class Logical : public Expr {
public:
    
    Expr    *expr1;
    Expr    *expr2;
    
    Logical(Token *t, Expr *e1, Expr *e2);
    
    Expr *gen();
    
    const char *toString();
    
    virtual Type *check(Type *p1, Type *p2);
};

class Or : public Logical {
    
public:
    
    Or(Token *t, Expr *e1, Expr *e2);
    
    void jumping(int t, int f);
};

class And : public Logical {
    
public:
    
    And(Token *t, Expr *e1, Expr *e2);
    
    void jumping(int t, int f);
};

class Not : public Logical {
    
public:
    
    Not(Token *t, Expr *e2);
    
    void jumping(int t, int f);
    
    const char *toString();
};

class Rel : public Logical {
    
public:
    
    Rel(Token *t, Expr *e1, Expr *e2);
        
    Type *check(Type *p1, Type *p2);
    
    void jumping(int t, int f);
};

// Statements
//
//  Seq:        sequence
//  If:         if
//  Else:       if-else
//  Break:      break
//  While:      while
//  Do:         do-while
//  Set:        non-array assignment
//  SetElem:    array assignment

class Stmt : public Node {
public:
    Stmt();
    
    virtual void gen(int b, int a);     // b - beginning of stmt, a -  after stmt
    
    static Stmt     *Null;
    static Stmt     *Enclosing;
    
    int after;                  // saved for break
};

class Seq : public Stmt {
private:
    Stmt *stmt1;
    Stmt *stmt2;
    
public:
    Seq(Stmt *s1, Stmt *s2);
    
    void gen(int b, int a);
};

class If : public Stmt {
private:
    Expr    *expr;
    Stmt    *stmt;
    
public:
    If(Expr *x, Stmt *s);
    
    void gen(int b, int a);
};

class Else : public Stmt {
private:
    Expr    *expr;
    Stmt    *stmt1;
    Stmt    *stmt2;
    
public:
    
    Else(Expr *e, Stmt *s1, Stmt *s2);
    
    void gen(int b, int a);
};

class Break : public Stmt {
private:
    Stmt    *stmt;
    
public:
    
    Break();
    
    void gen(int b, int a);
};

class While : public Stmt {
private:
    Expr    *expr;
    Stmt    *stmt;
    
public:
    
    While();
    
    void init(Expr *e, Stmt *s);
    
    void gen(int b, int a);
};

class Do : public Stmt {
private:
    Expr    *expr;
    Stmt    *stmt;
    
public:
    
    Do();
    
    void init(Expr *e, Stmt *s);
    
    void gen(int b, int a);
};

class Set : public Stmt {
public:
    Id      *id;
    Expr    *expr;
    
public:
    
    Set(Id *i, Expr *b);
    
    Type *check(Type *p1, Type *p2);
    
    void gen(int b, int a);
};

class SetElem : public Stmt {
    
public:
    Id      *array;
    Expr    *index;
    Expr    *expr;
    
    SetElem(Access *a, Expr *e);
    
    Type *check(Type *p1, Type *p2);
    
    void gen(int b, int a);
};

#endif /* defined(__FrontEnd__Node__) */
