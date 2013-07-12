/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: CommandLine.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "CommandLine.h"
#include <stdlib.h>
#include <stdio.h>

/**
   @brief Default constructor
 
   As instantiated, the analysis of the parameters takes place: 
   whenever the parser finds a syntax error or a misuse of the 
   above specifiers, parse immediately stops and the private 
   variable _fault is set to true, indicating the presence of 
   an error.
 
   @param[in]    argc    Number of available arguments
   @param[in]    argv    Array of parameters
 */
CommandLine::CommandLine(int argc, char * const argv[]) : 
                        _argc(argc), _argv(argv), _opts("u:p:s:d:")
{
    int ch;
    
    _fault = false;
    _debug = 0;
    _user = "root", _password = "secret", _server = "localhost";
    
    while ((ch = parseNext()) != EOF) {
        switch (ch) {
            case 'p': 
                _password = optionArgument();           
                break;
            case 's': 
                _server = optionArgument();         
                break;
            case 'u': 
                _user = optionArgument();           
                break;
            case 'd':
                _debug = atoi(optionArgument());
                break;
            default:
                parseError();
                return;
        }
    }
}

/**
   @brief Print an error message (syntax error)
 */
void CommandLine::parseError()
{
    cerr << "Unknown parameter!\n";
    printUsage();
    _fault = true;
}

/**
   @brief Print command line synopsis
 */
void CommandLine::printUsage() const 
{ 
    cerr << "usage: ec++ [ -u user ] [ -p password ] " \
            "[ -s server ] [-d level]\n\n";
}

/**
   @brief Returns database username

   @return A string representing database username
 */
const char * CommandLine::dbUser() const 
{ 
    return _fault?NULL:_user; 
}

/**
   @brief Returns database password
 
   @return A string representing database password
 */
const char * CommandLine::dbPasswd() const 
{ 
    return _fault?NULL:_password; 
}

/**
   @brief Returns database server address
 
   @return A string representing database server address
 */
const char * CommandLine::dbServer() const 
{ 
    return _fault?NULL:_server; 
}

/**
   @brief Returns debug log level
 
   @return An integer representing debug level
 */
int CommandLine::debugLevel()
{
    return _debug;
}

/**
   @brief Returns success of current option parse
 
   @return EOF if there are no more parameters to parse
 */
int CommandLine::parseNext() const
{
    return getopt(_argc, _argv, _opts);
}

/**
   @brief Returns the expected value of an option
 
   @return A string representing the option value
 */
const char * CommandLine::optionArgument() const
{
    return optarg;
}

/**
   @brief Returns parse exit status
 
   @return True if no syntax errors were detected
 */
bool CommandLine::isFault()
{
    return _fault;
}
