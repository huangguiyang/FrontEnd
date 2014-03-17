//
//  Node.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Node.h"
#include <stdio.h>
#include "Token.h"
#include "Error.h"
#include <typeinfo>

int Node::labels = 0;

Node::Node()
{
    
}

int Node::newlabel()
{
    return ++labels;
}

void Node::emitlabel(int l)
{
    fprintf(stdout, "L%d:", l);
}

void Node::emit(const char *s)
{
    fprintf(stdout, "\t%s\n", s);
}

Expr::Expr(Token *t, Type *p)
{
    this->op = t;
    this->type = p;
}

void Expr::jumping(int t, int f)
{
    emitjumps(toString(), t, f);
}

void Expr::emitjumps(const char *test, int t, int f)
{
    if (t != 0 && f != 0) {
        emit(fmtstr("if %s goto L%d", test, t));
        emit(fmtstr("goto L%d", f));
    }
    else if (t != 0) {
        emit(fmtstr("if %s goto L%d", test, t));
    }
    else if (f != 0) {
        emit(fmtstr("iffalse %s goto L%d", test, f));
    }
    else {
        // go through
    }
}

Expr *Expr::gen()
{
    return this;
}

Expr *Expr::reduce()
{
    return this;
}

const char *Expr::toString()
{
    return op->toString();
}

Id::Id(Word *w, Type *p, int b)
:Expr(w, p)
{
    this->offset = b;
}

Stmt *Stmt::Null = new Stmt();
Stmt *Stmt::Enclosing = Stmt::Null;

Stmt::Stmt()
{
    this->after = 0;
}

void Stmt::gen(int b, int a)
{
    // go through
}

Seq::Seq(Stmt *s1, Stmt *s2)
{
    stmt1 = s1;
    stmt2 = s2;
}

void Seq::gen(int b, int a)
{
    if (stmt1 == Stmt::Null) {
        stmt2->gen(b, a);
    }
    else if (stmt2 == Stmt::Null) {
        stmt1->gen(b, a);
    }
    else {
        
        int label = newlabel();
        stmt1->gen(b, label);
        emitlabel(label);
        stmt2->gen(label, a);
    }
}

If::If(Expr *x, Stmt *s)
{
    this->expr = x;
    this->stmt = s;
    
    if (expr->type != Type::Bool) {
        error("boolean required in if");
    }
}

void If::gen(int b, int a)
{
    int label = newlabel();
    expr->jumping(0, a);    // go through when true
    emitlabel(label);
    stmt->gen(label, a);
}

Else::Else(Expr *e, Stmt *s1, Stmt *s2)
{
    this->expr = e;
    this->stmt1 = s1;
    this->stmt2 = s2;
}

void Else::gen(int b, int a)
{
    int label1 = newlabel();
    int label2 = newlabel();
    
    expr->jumping(0, label2);
    emitlabel(label1);
    stmt1->gen(label1, a);
    emit(fmtstr("goto L%d", a));
    emitlabel(label2);
    stmt2->gen(label2, a);
}

Break::Break()
{
    if (Stmt::Enclosing == Stmt::Null) {
        error("unenclosing block");
    }
    
    this->stmt = Stmt::Enclosing;
}

void Break::gen(int b, int a)
{
    emit(fmtstr("goto L%d", stmt->after));
}

While::While()
{
    this->expr = NULL;
    this->stmt = NULL;
}

void While::init(Expr *e, Stmt *s)
{
    this->expr = e;
    this->stmt = s;
}

void While::gen(int b, int a)
{
    after = a;
    
    expr->jumping(0, a);
    int label = newlabel();
    emitlabel(label);
    stmt->gen(label, b);
    emit(fmtstr("goto L%d", b));
}

Do::Do()
{
    this->expr = NULL;
    this->stmt = NULL;
}

void Do::init(Expr *e, Stmt *s)
{
    this->expr = e;
    this->stmt = s;
}

void Do::gen(int b, int a)
{
    after = a;
    
    int label = newlabel();
    stmt->gen(b, label);
    emitlabel(label);
    expr->jumping(b, 0);
}

Op::Op(Token *t, Type *p)
:Expr(t, p)
{
    
}

Expr *Op::reduce()
{
    Expr *x = gen();
    Temp *t = new Temp(type);
    
    emit(fmtstr("%s = %s", t->toString(), x->toString()));
    
    return t;
}

Arith::Arith(Token *t, Expr *e1, Expr *e2)
:Op(t, NULL)
{
    this->expr1 = e1;
    this->expr2 = e2;
    
    type = Type::max(expr1->type, expr2->type);
    if (type == NULL) {
        error("type error at %s", t->toString());
    }
}

Expr *Arith::gen()
{
    return new Arith(op, expr1->reduce(), expr2->reduce());
}

const char *Arith::toString()
{
    return fmtstr("%s %s %s", expr1->toString(), op->toString(), expr2->toString());
}

Access::Access(Id *a, Expr *i, Type *p)
:Op(new Word("[]", Tag::INDEX), p)
{
    this->array = a;
    this->index = i;
}

Expr *Access::gen()
{
    return new Access(array, index->reduce(), type);
}

void Access::jumping(int t, int f)
{
    emitjumps(reduce()->toString(), t, f);
}

const char *Access::toString()
{
    return fmtstr("%s [%s]", array->toString(), index->toString());
}

int Temp::count = 0;

Temp::Temp(Type *p)
:Expr(Word::Temp, p)
{
    number = ++count;
}

const char *Temp::toString()
{
    return fmtstr("t%d", number);
}

Unary::Unary(Token *t, Expr *e)
:Op(t, NULL)
{
    this->expr = e;
    
    type = Type::max(Type::Int, expr->type);
    if (type == NULL) {
        error("type error at %s", t->toString());
    }
}

Expr *Unary::gen()
{
    return new Unary(op, expr->reduce());
}

const char *Unary::toString()
{
    return fmtstr("%s %s", op->toString(), expr->toString());
}

Set::Set(Id *i, Expr *b)
{
    this->id = i;
    this->expr = b;
    
    if (check(id->type, expr->type) == NULL) {
        error("type error");
    }
}

Type *Set::check(Type *p1, Type *p2)
{
    if (Type::numric(p1) && Type::numric(p2)) {
        return p2;
    }
    else if (p1 == Type::Bool && p2 == Type::Bool) {
        return p2;
    }
    else {
        return NULL;
    }
}

void Set::gen(int b, int a)
{
    emit(fmtstr("%s = %s", id->toString(), expr->gen()->toString()));
}

SetElem::SetElem(Access *a, Expr *e)
{
    this->array = a->array;
    this->index = a->index;
    this->expr = e;
    
    if (check(a->type, expr->type) == NULL) {
        error("type error");
    }
}

Type *SetElem::check(Type *p1, Type *p2)
{
    //instanceof
    if (typeid(p1) == typeid(Array) || typeid(p2) == typeid(Array)) {
        return NULL;
    }
    else if (p1 == p2) {
        return p2;
    }
    else if (Type::numric(p1) && Type::numric(p2)) {
        return p2;
    }
    else {
        return NULL;
    }
}

void SetElem::gen(int b, int a)
{
    const char *s1 = index->reduce()->toString();
    const char *s2 = expr->reduce()->toString();
    
    emit(fmtstr("%s[%s] = %s", array->toString(), s1, s2));
}

Constant *Constant::True = new Constant(Word::True, Type::Bool);
Constant *Constant::False = new Constant(Word::False, Type::Bool);

Constant::Constant(Token *t, Type *p)
:Expr(t, p)
{
    
}

Constant::Constant(int i)
:Expr(new Num(i), Type::Int)
{
    
}

void Constant::jumping(int t, int f)
{
    if (this == Constant::True && t) {
        emit(fmtstr("goto L%d", t));
    }
    else if (this == Constant::False && f) {
        emit(fmtstr("goto L%d", f));
    }
    else {
        // go through
    }
}

Logical::Logical(Token *t, Expr *e1, Expr *e2)
:Expr(t, NULL)
{
    this->expr1 = e1;
    this->expr2 = e2;
    
    //NOTE: calling virtual function in constructor has no effect
    //      moving to every subclass
}

Type *Logical::check(Type *p1, Type *p2)
{
    if (p1 == Type::Bool && p2 == Type::Bool) {
        return Type::Bool;
    }
    else {
        return NULL;
    }
}

Expr *Logical::gen()
{
    int f = newlabel();
    int a = newlabel();
    
    Temp *temp = new Temp(type);
    this->jumping(0, f);
    emit(fmtstr("%s = true", temp->toString()));
    emit(fmtstr("goto L%d", a));
    emitlabel(f);
    emit(fmtstr("%s = false", temp->toString()));
    emitlabel(a);
    
    return temp;
}

const char *Logical::toString()
{
    return fmtstr("%s %s %s", expr1->toString(), op->toString(), expr2->toString());
}

Or::Or(Token *t, Expr *e1, Expr *e2)
:Logical(t, e1, e2)
{
    type = check(expr1->type, expr2->type);
    if (type == NULL) {
        error("type error at %s", op->toString());
    }
}

void Or::jumping(int t, int f)
{
    int label = t != 0 ? t : newlabel();
    
    expr1->jumping(label, 0);
    expr2->jumping(t, f);
    
    if (t == 0) {
        emitlabel(label);
    }
}

And::And(Token *t, Expr *e1, Expr *e2)
:Logical(t, e1, e2)
{
    type = check(expr1->type, expr2->type);
    if (type == NULL) {
        error("type error at %s", op->toString());
    }
}

void And::jumping(int t, int f)
{
    int label = f != 0 ? f : newlabel();
    expr1->jumping(0, label);
    expr2->jumping(t, f);
    if (f == 0) {
        emitlabel(label);
    }
}

Not::Not(Token *t, Expr *e2)
:Logical(t, e2, e2)
{
    type = check(expr1->type, expr2->type);
    if (type == NULL) {
        error("type error at %s", op->toString());
    }
}

void Not::jumping(int t, int f)
{
    expr2->jumping(f, t);
}

const char *Not::toString()
{
    return fmtstr("%s %s", op->toString(), expr2->toString());
}

Rel::Rel(Token *t, Expr *e1, Expr *e2)
:Logical(t, e1, e2)
{
    type = check(expr1->type, expr2->type);
    if (type == NULL) {
        error("type error at %s", op->toString());
    }
}

Type *Rel::check(Type *p1, Type *p2)
{
    //instanceof
    if (typeid(p1) == typeid(Array) || typeid(p2) == typeid(Array)) {
        return NULL;
    }
    else if (p1 == p2) {
        return Type::Bool;
    }
    else {
        return NULL;
    }
}

void Rel::jumping(int t, int f)
{
    Expr *a = expr1->reduce();
    Expr *b = expr2->reduce();
    
    char *test = fmtstr("%s %s %s", a->toString(), op->toString(), b->toString());
    
    emitjumps(test, t, f);
}

