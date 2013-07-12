/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Exceptions.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <iostream>
#include <exception>

using namespace std;

/**
   @brief Bad Authentication/authorization exception
 
   @see UserMenu
 */
class BadAuthException : public exception
{
public:
    BadAuthException() {}
    virtual ~BadAuthException() throw() {}
    
    virtual const char *what() const throw() { 
        return "Authorization error"; 
    }
};

/**
   @brief Invalid argument exception
 
   @see ManagedObject
 */
class InvalidArgument : public exception 
{
private:
    string _arg;
    
public:
    InvalidArgument(string anArg) : _arg(anArg) {}
    virtual ~InvalidArgument() throw() {};
    
    virtual const char *what() const throw() { 
        string msg = "Invalid argument - " + _arg;
        return msg.c_str();
    }
};

#endif /*  __EXCEPTIONS_H__ */
