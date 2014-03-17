//
//  Token.h
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Token__
#define __FrontEnd__Token__

class Tag {
public:
    enum {
        AND = 256, BASIC, BREAK, DO, ELSE,
        EQ, FALSE, GE, ID, IF,
        INDEX, LE, MINUS, NE, NUM,
        OR, REAL, TEMP, TRUE, WHILE,
    };
};

class Token {
public:
    int tag;
    
    Token(int t);
    
    virtual const char *toString();
};

class Word : public Token {
public:
    const char *lexme;
    
    Word(const char *s, int tag);
    
    const char *toString();
    
public:
    static Word     *True;
    static Word     *False;
    static Word     *And;
    static Word     *Or;
    static Word     *Eq;
    static Word     *Ne;
    static Word     *Le;
    static Word     *Ge;
    static Word     *Minus;
    static Word     *Temp;
};

class Type : public Word {
public:
    int width;
    
public:
    Type(const char *s, int tag, int w);
    
    static Type *Int;
    static Type *Float;
    static Type *Char;
    static Type *Bool;
    
    static bool numric(Type *p);
    static Type *max(Type *p1, Type *p2);
};

class Array : public Type {
    
public:
    Type    *of;
    int     size;
    
    Array(int sz, Type *p);
    
    const char *toString();
};

class Num : public Token {
public:
    int value;
    
public:
    
    Num(int v);
    
    const char *toString();
};

class Real : public Token {
public:
    float value;
    
public:
    
    Real(float v);
    
    const char *toString();
};

#endif /* defined(__FrontEnd__Token__) */
