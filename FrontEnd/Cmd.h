//
//  Cmd.h
//  FrontEnd
//
//  Created by Mario Huang on 14-1-31.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#ifndef __FrontEnd__Cmd__
#define __FrontEnd__Cmd__

class Cmd {
public:
    static const char *ifile;
    
    static void process(int argc, const char *argv[]);
    
private:
    static void    _help();
    
    static  int argc;
    static  const char **argv;
};

#endif /* defined(__FrontEnd__Cmd__) */
