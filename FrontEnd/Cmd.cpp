//
//  Cmd.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Cmd.h"
#include <stdio.h>
#include <string.h>
#include "Error.h"

const char *Cmd::ifile = NULL;
int Cmd::argc = 0;
const char **Cmd::argv = NULL;

static const char *_verstr = "1.0";

void Cmd::process(int argc, const char *argv[])
{
    Cmd::argc = argc;
    Cmd::argv = argv;
    
    if (argc < 2) {
        _help();
        throw Error(NULL);
    }
    
    for (int i=1; i < argc; i++) {
        
        const char *arg = argv[i];
        
        if (!strcmp("-h", arg)) {
            _help();
        }
        else {
            Cmd::ifile = arg;
        }
    }
    
    if (Cmd::ifile == NULL) {
        throw Error("no input file");
    }
}

void Cmd::_help()
{
    fprintf(stderr,
            "A front-end described in 《Compilers: Principles, Techniques and Tools (Second Edition)》, C++ version source code.\n"\
            "Usage: program_name <filename> (version %s)\n",
            _verstr);
}
