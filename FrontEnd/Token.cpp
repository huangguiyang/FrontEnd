//
//  Token.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Token.h"
#include <stdio.h>
#include "Error.h"

Token::Token(int t)
{
    this->tag = t;
}

const char *Token::toString()
{
    return fmtstr("%c", tag);
}

Word *Word::True    = new Word("true",  Tag::TRUE);
Word *Word::False   = new Word("false", Tag::FALSE);
Word *Word::And     = new Word("&&",    Tag::AND);
Word *Word::Or      = new Word("||",    Tag::OR);
Word *Word::Eq      = new Word("==",    Tag::EQ);
Word *Word::Ne      = new Word("!=",    Tag::NE);
Word *Word::Le      = new Word("<=",    Tag::LE);
Word *Word::Ge      = new Word(">=",    Tag::GE);
Word *Word::Minus   = new Word("minus", Tag::MINUS);
Word *Word::Temp    = new Word("t",     Tag::TEMP);

Word::Word(const char *s, int tag)
:Token(tag)
{
    this->lexme = s;
}

const char *Word::toString()
{
    return lexme;
}

Type *Type::Int     = new Type("int",       Tag::BASIC, 4);
Type *Type::Float   = new Type("float",     Tag::BASIC, 8);
Type *Type::Char    = new Type("char",      Tag::BASIC, 1);
Type *Type::Bool    = new Type("bool",      Tag::BASIC, 1);

Type::Type(const char *s, int tag, int w)
:Word(s, tag)
{
    this->width = w;
}

bool Type::numric(Type *p)
{
    if (p == Type::Int || p == Type::Char || p == Type::Float) {
        return true;
    }
    else {
        return false;
    }
}

Type *Type::max(Type *p1, Type *p2)
{
    if (!numric(p1) || !numric(p2)) {
        return NULL;
    }
    else if (p1 == Type::Float || p2 == Type::Float) {
        return Type::Float;
    }
    else if (p1 == Type::Int || p2 == Type::Int) {
        return Type::Int;
    }
    else {
        return Type::Char;
    }
}

Array::Array(int sz, Type *p)
:Type("[]", Tag::INDEX, sz*p->width), size(1)
{
    this->size = sz;
    this->of = p;
}

const char *Array::toString()
{
    return fmtstr("[%d] %s", size, of->toString());
}

Num::Num(int v)
:Token(Tag::NUM)
{
    this->value = v;
}

const char *Num::toString()
{
    return fmtstr("%d", value);
}

Real::Real(float v)
:Token(Tag::REAL)
{
    this->value = v;
}

const char *Real::toString()
{
    return fmtstr("%f", value);
}

