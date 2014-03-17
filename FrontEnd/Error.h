//
//  Error.h
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Error__
#define __FrontEnd__Error__

#include <exception>

class Error : public std::exception {
public:
    
    Error(const char *description);
    
    const char *what() const throw();
    
protected:
    const char *head;
    const char *str;
};

extern void error(const char *fmt, ...);
extern void warnning(const char *fmt, ...);

extern char *fmtstr(const char *fmt, ...);

#endif /* defined(__FrontEnd__Error__) */
