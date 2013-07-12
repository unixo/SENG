/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Observer.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <string>

using namespace std;

// Forward declaration
class Observable;

/**
   The class Observer is the base class to derive in order to receive 
   notifications from observed object.
   An generic observer must first register itself as observer for an 
   attribute of an instance of a class: whenever this attribute is 
   changed, the observer is notified before and after value changed.
 
   @see Observable
 */
class Observer 
{
public:
    virtual ~Observer() {}
    
    virtual void willChangeValueForKey(string, Observable *) = 0;
    virtual void didChangeValueForKey(string, Observable *) = 0;
    virtual string & toString() = 0;
};

#endif /* __OBSERVER_H__ */
