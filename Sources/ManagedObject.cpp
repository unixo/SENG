/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: ManagedObject.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "ManagedObject.h"
#include "DataModel.h"

/**
   @brief Default constructor
 
   Construct an instance of ManagedObject linking with a corresponding 
   table on database named "anEntityName".
   The list of columns is fetched automatically.   
 */
ManagedObject::ManagedObject(string anEntityName)
{        
    LOG_CTOR();
    initEntity(anEntityName);
}

/**
   @brief Construct an instance from a fetched record
 
   Construct an instance of ManagedObject linking with a corresponding 
   table on database named "anEntityName" and reading data from
   mysqlpp::Row.
 */
ManagedObject::ManagedObject(string anEntityName, Row & aRow)
{
    LOG_CTOR();
    initEntity(anEntityName);
    
    set<string>::const_iterator it;
    
    for (it =_keys.begin(); it != _keys.end(); it++) {
        string key = *it;
        setValueForKey(key, (string) aRow[key.c_str()]);
    }
    
    _fault = false;
    _updatedKeys.clear();
}

/**
   @brief Default destructor
 */
ManagedObject::~ManagedObject() 
{
    LOG_DTOR();
}

/**
   @brief Internal init
 
   Build the list of keys (columns) for the specified entity (table)
 */
void ManagedObject::initEntity(string anEntityName)
{
    _entityName = anEntityName;
    _lastInsertID = 0;
    _fault = false;    
    
    _keys = DataModel::instance().keysForEntity(anEntityName);
}

/**
   Sets the specified property of the receiver to the specified 
   string value.
  
   @param[in]    aKey   The name of one of the receiver's properties.
   @param[in]    aValue The new value for the property specified by 
                       key.
   @exception InvalidArgument Thrown if the specified key doesn't 
                       belong to this entity
   @see Observable, willChangeValueForKey, didChangeValueForKey
 */
void ManagedObject::setValueForKey(string aKey, 
                                   string aValue) throw (InvalidArgument)
{
    // first check if the key is valid, anyway throw an exception
    set<string>::const_iterator it = _keys.find(aKey);
    if (it == _keys.end())
        throw InvalidArgument(aKey);
    
    // Set the value only if differs from the old one
    if (aValue != *it) {        
        // notify observers that this key is going to change
        willChangeValueForKey(aKey);
        
        _fields[aKey] = aValue;
        _fault = true;
        _updatedKeys.insert(aKey);
        
        // notify observers that this key is changed
        didChangeValueForKey(aKey);        
    }
}

/**
   @brief Set an integer value
 
   Sets the specified property of the receiver to the specified 
   integer value.
 
   @param[in]    aKey   The name of one of the receiver's 
                       properties.
   @param[in]    aValue The new value for the property specified 
                       by key. 
 */
void ManagedObject::setIntForKey(string aKey, int aValue)
{
    setFloatForKey(aKey, (float)aValue);
}

/**
   @brief Set a float value
 
   Sets the specified property of the receiver to the specified 
   float value.
 
   @param[in]    aKey   The name of one of the receiver's 
                       properties.
   @param[in]    aValue The new value for the property specified 
                       by key.
 */
void ManagedObject::setFloatForKey(string aKey, float aValue)
{
    stringstream tmpSS;
    string tmpStr;
    
    tmpSS << aValue;
    tmpSS >> tmpStr;
    setValueForKey(aKey, tmpStr);
}

/**
   @brief Set a boolean value
 
   Sets the specified property of the receiver to the specified 
   boolean value.
 
   @param[in]    aKey   The name of one of the receiver's 
                       properties.
   @param[in]    aValue The new value for the property specified 
                       by key.
 */
void ManagedObject::setBoolForKey(string aKey, bool aValue)
{
    setValueForKey(aKey, (aValue?"1":"0"));
}

/**
   @brief Get an attribute value
 
   Returns the value as string for the property identified by a 
   given key.
 
   @param[in]    aKey The key to read from
   @return Value of the property
 
   @exception InvalidArgument Thrown if the specified key doesn't 
                belong to this entity 
*/
string ManagedObject::valueForKey(string aKey) throw (InvalidArgument)
{
    // first check if the key is valid, anyway throw an exception
    set<string>::const_iterator it = _keys.find(aKey);
    if (it == _keys.end())
        throw InvalidArgument(aKey);
    
    return _fields[aKey];
}

/**
   @brief Returns the value as boolean for the property identified 
          by a given key.
 
   @param[in]    aKey The key to read from
   @return Value of the property
   @exception InvalidArgument Thrown if the specified key doesn't 
                belong to this entity 
 */
bool ManagedObject::boolForKey(string aKey) throw (InvalidArgument)
{
    string aValue = valueForKey(aKey);
    istringstream aStream(aValue);
    int i;
    
    if (!(aStream >> i))
        throw InvalidArgument(aKey);
    
    return (i == 1);
}

/**
   @brief Returns the value as integer for the property identified 
          by a given key.
 
   @param[in]    aKey The key to read from
   @return Value of the property
   @exception InvalidArgument Thrown if the specified key doesn't 
                     belong to this entity 
 */
int ManagedObject::intForKey(string aKey) throw (InvalidArgument)
{
    string value = valueForKey(aKey);
    istringstream aStream(value);
    int i;
    
    if (!(aStream >> i))
        throw InvalidArgument(aKey);
    
    return i;
}

/**
   @brief Returns the value as float for the property identified 
          by a given key.
 
   @param[in]    aKey The key to read from
   @return Value of the property
   @exception InvalidArgument Thrown if the specified key doesn't 
                     belong to this entity 
 */
float ManagedObject::floatForKey(string aKey) throw (InvalidArgument)
{
    string value = valueForKey(aKey);
    istringstream aStream(value);
    int f;
    
    if (!(aStream >> f))
        throw InvalidArgument(aKey);
    
    return f;
}

/**
   @brief Add this istance to database
 
   Attempts to commit unsaved changes to the persistent store: 
   this function builds a REPLACE statement.
   Fault state is reset if update was successful.
 
   @note ManagedObject::store should be called only to store new 
         record and not an existing one.
 
   @return    True if save was successful
   @see    update
 */
bool ManagedObject::store()
{    
    SQLQueryParms qp;
    string cols = " (";
    stringstream values;
    int i=0;
    
    /**
       Iterate on all key/value in order to create the VALUE() part 
       of REPLACE statement; we also fill a SQLQueryParams to pass to 
       mysqlpp::query::execute.
     
       We don't make use of valueMerge template method becuase we 
       need to know on which field we're iterating, in order to 
       properly fill "qp" and "values".
     */
    values << "VALUES (";
    set<string>::reverse_iterator it;
    for (it = _keys.rbegin(); it != _keys.rend(); it++) {
        cols += *it + ",";
        qp += _fields[*it];
        values << "%" << i++ << "q,";
    }
    
    // build the entire REPLACE statement
    cols[cols.length()-1] = ')', cols += " ";
    string sql = "REPLACE " + _entityName + cols + values.str();
    sql[sql.length()-1] = ')';
    
    // get an instance of the database
    Database & db = Database::instance();
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query(sql);
        q.parse();
        
        LOG(2, "SQL: %s\n", q.str(qp).c_str());

        // check if command was successful: if not, log the error 
        // and its detail
        SimpleResult r = q.execute(qp);
        if (r == false) {
            LOG(2, "An error occurred during mysqlpp:query::execute\n"
                   "ERR: %s\nQuery was: %s\n", q.error(), 
                   q.str(qp).c_str());
                
            return false;
        }
        
        _lastInsertID = q.insert_id();
    }
    catch (const Exception& er) {
        cerr << "Error: " << er.what() << endl;        
        return false;
    }    
    
    // restore fault state
    _fault = false;
    
    return true;
}

/**
   @brief Make changes persistent to database.
 
   Attempts to update unsaved changes to the persistent store.
   Fault state is reset if update is successful.
 
   @note ManagedObject::update should be called only to update 
         existing record and not an to store a new record.
 
   @return    True if update was successful
   @see    store
 */
bool ManagedObject::update()
{
    if (!_fault)
        return true;
    
    SQLQueryParms qp;
    set<string>::const_iterator ckit;
    vector<string> vValues;
    stringstream aValue;
    string sql;
    int i;
    
    /**
          We don't make use of valueMerge template method becuase 
       we need to know on which field we're iterating, in order to 
       properly fill "qp" and "values".
    */
    for (i=0, ckit = _updatedKeys.begin(); 
         ckit != _updatedKeys.end(); ckit++, i++) {
        aValue.str("");
        aValue << *ckit << "=%" << i << "q";
        vValues.push_back(aValue.str());
        qp += _fields[*ckit];
    }
    
    sql = "UPDATE " + _entityName + " SET " + 
            valueMerge(vValues.begin(), vValues.end(), (string)",");

    // get the primary key of the entity
    string pk = primaryKey();
    
    // add the WHERE condition to properly identify the right record
    sql += " WHERE " + pk + " = " + _fields[pk];

    // get an instance of the database
    Database& db = Database::instance();
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query(sql);
        
        LOG(2, "SQL: %s\n", q.str(qp).c_str());
        
        q.parse();
        
        SimpleResult r = q.execute(qp);
        if (r == false) {
            cerr << "An error occurred during Person::store()\n" << endl 
            << q.error() << endl << "Query was: " << q.str() << endl;
            return false;
        }
    }
    catch (const mysqlpp::BadQuery& e) {
        // Something went wrong with the SQL query.
        cerr << "Query failed: " << e.what() << endl;
        return false;
    }
    catch (const Exception& er) {
        cerr << "Error: " << er.what() << endl;
        return false;
    }    
    
    _fault = false;
    
    return true;
}

/**
   @brief Returns the ID of the autoincrement of the last query (INSERT)
 
   @return    The last ID
 */
ulonglong ManagedObject::getLastInsertID() const
{ 
    return _lastInsertID;
}
