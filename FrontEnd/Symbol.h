//
//  Symbol.h
//  FrontEnd
//
//  Created by Mario Huang on 14-2-1.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Symbol__
#define __FrontEnd__Symbol__

#include <map>

class Token;
class Id;

class Env {
private:
    std::map<Token*, Id*>   table;
    
protected:
    Env *prev;
    
public:
    
    Env(Env &v);
    
    void put(Token *t, Id *i);
    Id *get(Token *t);
};

#endif /* defined(__FrontEnd__Symbol__) */
