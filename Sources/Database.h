/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Database.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <mysql++.h>
#include <query.h>
#include "common.h"

using namespace std;
using namespace mysqlpp;

/**
   Manages the connection to the database server.
   The class is intented to be used as singleton: call method 
   instance() to get an instance of Database: for this reason, 
   Database needs to be initialized early, such as in your main, 
   with default parameter (use private constructor).
    
   @see Singleton
 */
class Database : public Singleton<Database>
{
private:    
    Connection *_conn;
    string _server;
    string _user;
    string _passwd;
    string _db;
    
protected:
    friend class Singleton<Database>;
    Database();
    
    void printRow(IntVector & widths, Row& row);
    
public:
    virtual ~Database();
        
    bool isConnected();
    bool connect();
    void disconnect();    
    Connection *getConnection();
    void printResult(StoreQueryResult& res);
    
    void setServer(string aValue);
    void setUser(string aValue);
    void setPassword(string aValue);
    void setDB(string aValue);
    
    friend ostream& operator<<(ostream &, Database &);
};    

#endif /* __DATABASE_H__ */
