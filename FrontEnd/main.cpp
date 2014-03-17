//
//  main.cpp
//  FrontEnd
//
//  Created by Mario Huang on 14-1-30.
//  Copyright (c) 2014 Mario Huang. All rights reserved.
//

#include <iostream>
#include "Parser.h"
#include "Lexer.h"
#include "Cmd.h"
#include "Error.h"

int main(int argc, const char * argv[])
{
    int ret = EXIT_SUCCESS;
    
    try {
        Cmd::process(argc, argv);
        
        Lexer *lexer = new Lexer();
        Parser *parer = new Parser(lexer);
        
        parer->program();
        
    } catch (std::exception &e) {
        
        ret = EXIT_FAILURE;
        
        if (e.what()) {
            std::cout << e.what() << std::endl;
        }
        
    } 
    
    return ret;
}

