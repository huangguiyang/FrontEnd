//
//  Lexer.h
//  FrontEnd
//
//  Created by Mario Huang on 14-1-30.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Lexer__
#define __FrontEnd__Lexer__

#include <map>
#include <fstream>
#include "Token.h"

struct cmp_cls {
    bool operator() (const char *a, const char *b) const {
        return std::strcmp(a, b) < 0;
    }
};

class Lexer {
private:
    std::map<const char*, Word*, cmp_cls> words;
    std::ifstream   *ifs;
    char    *buffer;
    char    *pCurrent;
    char    peek;
    
    void reserve(Word *w);
    void fillBuffer();
    void readch();
    bool readch(char c);
    
public:
    static  int line;
    
    Lexer();
    
    Token *gettok();
    
};

#endif /* defined(__FrontEnd__Lexer__) */
