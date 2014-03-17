//
//  Error.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include "Error.h"
#include "Cmd.h"
#include <stdio.h>
#include "Lexer.h"

Error::Error(const char *description)
{
    this->str = description;
}

const char *Error::what() const throw()
{
    return this->str;
}

void error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    char *str;
    vasprintf(&str, fmt, ap);
    
    va_end(ap);
    
    char *ret;
    asprintf(&ret, "[Error]%s:Line %d: %s\n", Cmd::ifile, Lexer::line, str);
    
    throw Error(ret);
}

void warnning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    char *str;
    vasprintf(&str, fmt, ap);
    
    va_end(ap);
    
    fprintf(stderr, "[Warnning]%s:Line %d: %s\n", Cmd::ifile, Lexer::line, str);
}

char *fmtstr(const char *fmt, ...)
{
    char *ret = NULL;
    
    va_list ap;
    va_start(ap, fmt);
    
    vasprintf(&ret, fmt, ap);
    
    va_end(ap);
    
    return ret;
}

