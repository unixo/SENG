/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Observable.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Observable.h"
#include "Observer.h"

/**
   @brief Default constructor
 */
Observable::Observable()
{
    LOG_CTOR();
}

/**
   @brief Default destructor
 */
Observable::~Observable()
{
    LOG_DTOR();
}

/**
   @brief Register an observer
 
   Adds an entry to the receiver’s dispatch table with an observer,
   for a notifications restricted to key.
 
   @param[in]    aKey The name of the notification for which to register
                      the observer
   @param[in]    o    Object registering as an observer. 
                      This value must not be NULL
 */
void Observable::addObserver(std::string aKey, Observer & o)
{
    map<string, set<Observer *> >::iterator it = _observers.find(aKey);
    set<Observer *> *aSet;
    
    LOG(3, "Added new observer: %s\n", o.toString().c_str());
    
    if (it != _observers.end())
        aSet = &(*it).second;
    else
        aSet = new set<Observer *>();
    
    aSet->insert(&o);
    _observers[aKey] = *aSet;
}

/**
   @brief Unregister an observer
 
   Removes matching entries from the receiver’s dispatch table for the 
   specified keys.
 
   @param[in]    aKey  The name of the notification for which to remove
					   the observer
   @param[in]    o     Observer to remove from the dispatch table
 */
void Observable::removeObserver(string aKey, Observer& o)
{
    map<string, set<Observer *> >::iterator it = _observers.find(aKey);
    if (it == _observers.end())
        return;
    
    LOG(3, "Removed observer: %s\n", o.toString().c_str());
    
    set<Observer *> *aSet = &(*it).second;
    aSet->erase(aSet->find(&o));
    _observers[aKey] = *aSet;
}

/**
    @brief Remove all observer from the dispatch table.
 */
void Observable::removeAllObservers() 
{
    _observers.clear();
}

/**
   @brief Returns the number of keys observed.

   @return    Count of keys.
 */
int Observable::countObservers()
{
    return _observers.size();
}

/**
   @brief Notification before value changes
 
   Invoked to inform the receiver that the value of a given property 
   is about to change. 

   @return    aKey The name of the property that will change.
 */
void Observable::willChangeValueForKey(string aKey)
{
    map<string, set<Observer *> >::iterator it = _observers.find(aKey);
    if (it == _observers.end())
        return;
    
    LOG(3, "willChangeValueForKey('%s')\n", aKey.c_str());
    
    set<Observer *> *aSet = &(*it).second;
    set<Observer *>::iterator sit;
    for (sit = aSet->begin(); sit != aSet->end(); sit++)
        (*sit)->willChangeValueForKey(aKey, this);
}

/**
   @brief Notification after value changed
 
   Invoked to inform the receiver that the value of a given property 
   has changed.
 
   @return    aKey The name of the property that changed.
 */
void Observable::didChangeValueForKey(string aKey)
{
    map<string, set<Observer *> >::iterator it = _observers.find(aKey);
    if (it == _observers.end())
        return;
    
    LOG(3, "didChangeValueForKey('%s')\n", aKey.c_str());
    
    set<Observer *> *aSet = &(*it).second;
    set<Observer *>::iterator sit;
    for (sit = aSet->begin(); sit != aSet->end(); sit++)
        (*sit)->didChangeValueForKey(aKey, this);
}
