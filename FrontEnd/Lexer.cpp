//
//  Lexer.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-30.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Lexer.h"
#include "Cmd.h"
#include "Error.h"

using std::string;
using std::ifstream;

#define kBufSize    4096

int Lexer::line = 0;

Lexer::Lexer()
{
    ifs = new ifstream(Cmd::ifile);
    if (!ifs->good()) {
        throw Error(fmtstr("Cannot open file \"%s\"", Cmd::ifile));
    }
    
    reserve(new Word("if",      Tag::IF));
    reserve(new Word("else",    Tag::ELSE));
    reserve(new Word("while",   Tag::WHILE));
    reserve(new Word("do",      Tag::DO));
    reserve(new Word("break",   Tag::BREAK));
    
    reserve(Word::True);
    reserve(Word::False);
    reserve(Type::Int);
    reserve(Type::Float);
    reserve(Type::Char);
    reserve(Type::Bool);
    
    buffer = new char[kBufSize+1];
    pCurrent = buffer;
    buffer[kBufSize] = EOF;
    peek = ' ';
    line = 1;
    
    fillBuffer();
}

void Lexer::reserve(Word *w)
{
    words[w->lexme] = w;
}

void Lexer::fillBuffer()
{
    if (ifs->eof()) {
        return;
    }
    
    char *pbuf = buffer;
    pCurrent = buffer;
    ifs->read(pbuf, kBufSize);
        
    if (ifs->eof()) {
        
        pbuf[ifs->gcount()] = EOF;
        
        ifs->close();
    }
    else {
        
        
    }
}

void Lexer::readch()
{
    if (*pCurrent == EOF) {
        fillBuffer();
    }
    
    if (*pCurrent == EOF) {
        peek = EOF;
    }
    else {
        peek = *pCurrent++;
    }
}

bool Lexer::readch(char c)
{
    readch();
    if (peek != c) {
        return false;
    }
    peek = ' ';
    return true;
}

Token *Lexer::gettok()
{
    for (; ; readch()) {
        if (peek == ' ' || peek == '\t') {
            continue;
        }
        else if (peek == '\n') {
            line++;
        }
        else {
            break;
        }
    }
    
    switch (peek) {
        case '&':
            if (readch('&')) {
                return Word::And;
            }
            else {
                return new Token('&');
            }
            
        case '|':
            if (readch('|')) {
                return Word::Or;
            }
            else {
                return new Token('|');
            }
            
        case '=':
            if (readch('=')) {
                return Word::Eq;
            }
            else {
                return new Token('=');
            }
            
        case '!':
            if (readch('=')) {
                return Word::Ne;
            }
            else {
                return new Token('!');
            }
            
        case '>':
            if (readch('=')) {
                return Word::Ge;
            }
            else {
                return new Token('>');
            }
            
        case '<':
            if (readch('=')) {
                return Word::Le;
            }
            else {
                return new Token('<');
            }
    }
    
    if (isdigit(peek)) {
        
        int v = 0;
        
        do {
            v = 10*v + (peek - '0');
            readch();
        } while (isdigit(peek));
        
        if (peek != '.') {
            return new Num(v);
        }
        else {
            float x = v;
            float d = 10;
            for (; ; ) {
                readch();
                if (!isdigit(peek)) break;
                
                x = x + (peek - '0')/d;
                d = d * 10;
            }
            
            return new Real(x);
        }
        
    }
    
    if (isalpha(peek)) {
        
        string *s = new string();
        
        do {
            s->append(&peek);
            readch();
        } while (isalpha(peek) || isdigit(peek));
        
        Word *w = words[s->c_str()];
        if (w != NULL) {
            return w;
        }
        
        w = new Word(s->c_str(), Tag::ID);
        words[s->c_str()] = w;
        
        return w;
    }
    
    Token *tok = new Token(peek);
    peek = ' ';
    
    return tok;
}


