/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: CommandLine.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__

#include <string>
#include <iostream>

using namespace std;

/**
   @brief Command line parser
 
   The class CommandLine is an helper class used to parse 
   command line arguments passed to the executable. The main 
   usage of the class is suggested by its constructor, which 
   expects two parameters, the argc and argv  of the main(), 
   the entry point of any program.
 
   As instantiated, the analysis of the parameters takes place: 
   whenever the parser finds a syntax error or a misuse of the 
   above specifiers, parse immediately stops and the private 
   variable _fault is set to true, indicating the presence of an 
   error. Whenever an error is found, an help message is 
   displayed to the user, with the expected syntax to be used.
 */
class CommandLine 
{
private:
    bool _fault;
    int _argc;
    char * const* _argv;
    const char *_opts;    
    const char *_password;
    const char *_user;
    const char *_server;
    int _debug;
    
protected:
    const char * optionArgument() const;
    int parseNext() const;
    void parseError();
    
public:
    CommandLine(int argc, char * const argv[]);

    void printUsage() const;
    const char * dbUser() const;
    const char * dbPasswd() const;
    const char * dbServer() const;
    int debugLevel();
    bool isFault();

};

#endif /* __COMMANDLINE_H__ */
