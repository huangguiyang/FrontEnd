//
//  Symbol.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-2-1.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Symbol.h"

using std::map;

Env::Env(Env &v)
{
    prev = &v;
}

void Env::put(Token *t, Id *i)
{
    table[t] = i;
}

Id *Env::get(Token *t)
{
    for (Env *e = this; e != NULL; e = e->prev) {
        
        Id *found = e->table[t];
        if (found != NULL) {
            return found;
        }
    }
    
    return NULL;
}

