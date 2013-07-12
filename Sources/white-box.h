/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: white-box.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "common.h"
#include "Observer.h"
#include "Observable.h"
#include "User.h"

class TestObserver : public Observer 
{
private:
    string _name;
    
public:
    TestObserver(string s) : _name(s) { };
    
    virtual void willChangeValueForKey(string k, Observable *) { 
        cout << "[TestObserver::willChangeValueForKey] key: '" << k 
             << "' observer: '" << _name << "'\n"; 
    };
    
    virtual void didChangeValueForKey(string k, Observable *) {
        cout << "[TestObserver::didChangeValueForKey] key: '" << k 
             << "' observer: '" << _name << "'\n"; 
    };
    
    string & toString() { return _name; };
};

class TestObservable : public Observable
{
private:
    string _attr;
    
public:
    void setKey(string &aValue) {
        willChangeValueForKey("attr");
        cout << "[TestObservable::setKey] setting new value\n";
        _attr = aValue;
        didChangeValueForKey("attr");
    };
};
