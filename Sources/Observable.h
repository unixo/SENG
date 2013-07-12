/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Observable.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#include "common.h"

using namespace std;

// Forward declaration
class Observer;

/**
   The class Observable defines a mechanism that allows objects to be 
   notified of changes to the specified properties of other objects.
 
   You can observe any object properties including simple attributes. 
   Observers are informed of the type of change made — as well as which 
   objects are involved in the change.
 */
class Observable
{
private:
    /** Association between the key and the list of observers */
    map<string, set<Observer *> > _observers;

protected:
    virtual void willChangeValueForKey(string aKey);
    virtual void didChangeValueForKey(string aKey);
    
public:
    Observable();
    virtual ~Observable();

    virtual void addObserver(string aKey, Observer & o);    
    virtual void removeObserver(string aKey, Observer & o);    
    virtual void removeAllObservers();    
    virtual int countObservers();
};

#endif /* __OBSERVABLE_H__ */
