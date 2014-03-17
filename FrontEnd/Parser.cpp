//
//  Parser.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-30.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Parser.h"
#include "Lexer.h"
#include "Error.h"
#include <stdio.h>
#include "Cmd.h"
#include "Node.h"
#include "Symbol.h"

Parser::Parser(Lexer *l)
{
    top = NULL;
    used = 0;
    
    this->lexer = l;
    move();
}

void Parser::move()
{
    look = lexer->gettok();
}

void Parser::match(int t)
{
    if (look->tag == t) {
        move();
    }
    else {
        error("syntax error: '%c' expected", t);
    }
}

void Parser::program()
{
    Stmt *s = block();
    int begin = s->newlabel();
    int after = s->newlabel();
    
    s->emitlabel(begin);
    s->gen(begin, after);
    s->emitlabel(after);
}

Stmt *Parser::block()
{
    match('{');
    
    Env *savedEnv = top;
    
    top = new Env(*top);
    
    decls();
    
    Stmt *s = stmts();
    
    match('}');
    
    top = savedEnv;
    
    return s;
}

void Parser::decls()
{
    while (look->tag == Tag::BASIC) {
        
        Type *p = type();
        Token *tok = look;
        match(Tag::ID);
        match(';');
        
        Id *id = new Id((Word *)tok, p, used);
        top->put(tok, id);
        
        used = used + p->width;
    }
}

Type *Parser::type()
{
    Type *p = (Type *) look;
    match(Tag::BASIC);
    
    if (look->tag != '[') {
        return p;
    }
    else {
        return dims(p);
    }
}

Type *Parser::dims(Type *p)
{
    match('[');
    Token *tok = look;
    match(Tag::NUM);
    match(']');
    
    if (look->tag == '[') {
        p = dims(p);
    }
    
    return new Array(((Num *)tok)->value, p);
}

Stmt *Parser::stmts()
{
    if (look->tag == '}') {
        return Stmt::Null;
    }
    else {
        return new Seq(stmt(), stmts());
    }
}

Stmt *Parser::stmt()
{
    Expr *x;
    Stmt *s, *s1, *s2;
    Stmt *savedStmt;
    While *whilenode;
    Do *donode;
    
    switch (look->tag) {
        case ';':
            move();
            return Stmt::Null;
            
        case Tag::IF:
            match(Tag::IF);
            match('(');
            x = boolean();
            match(')');
            s1 = stmt();
            
            if (look->tag != Tag::ELSE) {
                return new If(x, s1);
            }
            
            match(Tag::ELSE);
            s2 = stmt();
            
            return new Else(x, s1, s2);
            
        case Tag::WHILE:
            
            whilenode = new While();
            savedStmt = Stmt::Enclosing;
            Stmt::Enclosing = whilenode;
            
            match(Tag::WHILE);
            match('(');
            x = boolean();
            match(')');
            
            s = stmt();
            
            whilenode->init(x, s);
            
            Stmt::Enclosing = savedStmt;
            
            return whilenode;
            
        case Tag::DO:
            
            donode = new Do();
            savedStmt = Stmt::Enclosing;
            Stmt::Enclosing = donode;
            
            match(Tag::DO);
            s1 = stmt();
            match(Tag::WHILE);
            
            match('(');
            x = boolean();
            match(')');
            match(';');
            
            donode->init(x, s1);
            
            Stmt::Enclosing = savedStmt;
            
            return donode;
            
        case Tag::BREAK:
            match(Tag::BREAK);
            match(';');
        
            return new Break();
            
        case '{':
            return block();
            
        default:
            
            return assign();
            
    }
    
}

Stmt *Parser::assign()
{
    Stmt *stmt;
    Token *t = look;
    
    match(Tag::ID);
    Id *id = top->get(t);
    if (id == NULL) {
        error("'%s' undeclared", t->toString());
    }
    
    if (look->tag == '=') {
        move();
        
        stmt = new Set(id, boolean());
    }
    else {
        
        Access *x = offset(id);
        
        match('=');
        
        stmt = new SetElem(x, boolean());
    }
    
    match(';');
    
    return stmt;
}

Expr *Parser::boolean()
{
    Expr *x = join();
    
    while (look->tag == Tag::OR) {
        
        Token *tok = look;
        move();
        
        x = new Or(tok, x, join());
        
    }
    
    return x;
}

Expr *Parser::join()
{
    Expr *x = equality();
    
    while (look->tag == Tag::AND) {
        
        Token *tok = look;
        move();
        
        x = new And(tok, x, equality());
        
    }
    
    return x;
}

Expr *Parser::equality()
{
    Expr *x = rel();
    
    while (look->tag == Tag::EQ || look->tag == Tag::NE) {
        
        Token *tok = look;
        move();
        
        x = new Rel(tok, x, rel());
    }
    
    return x;
}

Expr *Parser::rel()
{
    Expr *x = expr();
    
    switch (look->tag) {
        case '<':
        case '>':
        case Tag::LE:
        case Tag::GE:
        {
            Token *tok = look;
            move();
            
            return new Rel(tok, x, expr());
        }
            
        default:
            return x;
    }
}

Expr *Parser::expr()
{
    Expr *x = term();
    
    while (look->tag == '+' || look->tag == '-') {
        Token *tok = look;
        move();
        
        x = new Arith(tok, x, term());
    }
    
    return x;
}

Expr *Parser::term()
{
    Expr *x = unary();
    
    while (look->tag == '*' || look->tag == '/') {
        Token *tok = look;
        move();
        
        x = new Arith(tok, x, unary());
    }
    
    return x;
}

Expr *Parser::unary()
{
    if (look->tag == '-') {
        move();
        
        return new Unary(Word::Minus, unary());
    }
    else if (look->tag == '!') {
        Token *tok = look;
        move();
        
        return new Not(tok, unary());
    }
    else {
        return factor();
    }
}

Expr *Parser::factor()
{
    Expr *x = NULL;
    
    switch (look->tag) {
        case '(':
            match('(');
            x = boolean();
            match(')');
            
            return x;
            
        case Tag::NUM:
            x = new Constant(look, Type::Int);
            move();
            
            return x;
            
        case Tag::REAL:
            x = new Constant(look, Type::Float);
            move();
            
            return x;
            
        case Tag::TRUE:
            x = Constant::True;
            move();
            
            return x;
            
        case Tag::FALSE:
            x = Constant::False;
            move();
            
            return x;
            
        case Tag::ID:
        {
            Id *id = top->get(look);
            if (id == NULL) {
                error("'%s' undeclared", look->toString());
            }
            
            move();
            
            if (look->tag != '[') {
                return id;
            }
            else {
                return offset(id);
            }
        }
            
        default:
            error("syntax error at '%s'", look->toString());
            return x;
    }
}

Access *Parser::offset(Id *a)
{
    Expr *i, *w;
    Expr *t1, *t2;
    Expr *loc;
    
    Type *type = a->type;
    
    match('[');
    i = boolean();
    match(']');
    
    type = ((Array *) type)->of;
    w = new Constant(type->width);
    t1 = new Arith(new Token('*'), i, w);
    loc = t1;
    
    while (look->tag == '[') {
        match('[');
        i = boolean();
        match(']');
        
        type = ((Array *) type)->of;
        w = new Constant(type->width);
        t1 = new Arith(new Token('*'), i, w);
        t2 = new Arith(new Token('+'), loc, t1);
        loc = t2;
    }
    
    return new Access(a, loc, type);
}

