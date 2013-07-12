/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: ManagedObject.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __MANAGEDOBJECT_H__
#define __MANAGEDOBJECT_H__

#include "Observable.h"
#include "Exceptions.h"
#include "Database.h"

using namespace std;
using namespace mysqlpp;


/**
   The abstract class ManagedObject is a generic class that implements 
   all the basic behaviour of a table of the database. So, a managed 
   object is associated with an entity of the database ; this information 
   must be passed to the constructor of the class: when an instance of
   the class is created, a connection to the database is automatically 
   requested in order to fetch all the available columns of the linked
   table.
 */
class ManagedObject : public Observable
{
private:
    ulonglong _lastInsertID;
    void initEntity(string anEntityName);
    
protected:
    /** List of possible keys associated with this entity */
    set<string> _keys;
    /** List of the updated keys to update */
    set<string> _updatedKeys;
    /** Map of key/value associated to the entity */
    map<string, string> _fields;
    /** The entity name */
    string _entityName;
    /** Fault state: if true the entity need to be serialized */
    bool _fault;
    
public:
    ManagedObject(string anEntityName);
    ManagedObject(string anEntityName, Row & aRow);
    virtual ~ManagedObject();
    
    void setBoolForKey(string aKey, bool aValue);
    void setFloatForKey(string aKey, float aValue);
    void setIntForKey(string aKey, int aValue);
    void setValueForKey(string aKey, string aValue) throw (InvalidArgument);
    
    float floatForKey(string aKey) throw (InvalidArgument);
    int intForKey(string aKey) throw (InvalidArgument);
    bool boolForKey(string aKey) throw (InvalidArgument);
    string valueForKey(string aKey) throw (InvalidArgument);
    
    ulonglong getLastInsertID() const;
    bool store();
    bool update();
    
    virtual string primaryKey() = 0;
};

#endif /* __MANAGEDOBJECT_H__ */
