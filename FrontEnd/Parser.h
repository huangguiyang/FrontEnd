//
//  Parser.h
//  FrontEnd
//
//  Created by Mario Huang on 14-1-30.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Parser__
#define __FrontEnd__Parser__

class Lexer;
class Token;
class Stmt;
class Env;
class Type;
class Expr;
class Access;
class Id;

class Parser {
private:
    Lexer   *lexer;
    Token   *look;
    Env     *top;
    int     used;
    
    void    move();
    void    match(int t);
    
    // parse
private:
    Stmt    *block();
    void    decls();
    Stmt    *stmts();
    Stmt    *stmt();
    Type    *type();
    Type    *dims(Type *p);
    Expr    *boolean();
    Stmt    *assign();
    Expr    *join();
    Expr    *equality();
    Expr    *rel();
    Expr    *expr();
    Expr    *term();
    Expr    *unary();
    Expr    *factor();
    Access  *offset(Id *a);
    
public:
    Parser(Lexer *l);
    
    void    program();
};

#endif /* defined(__FrontEnd__Parser__) */
