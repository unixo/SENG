/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Database.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Database.h"

/**
   @brief Default constructor
 
   Init the class specifying the database informations (server to 
   connect to, credentials to authenticate and database name).
 */
Database::Database()
{
    LOG_CTOR();
    setServer("localhost");
    setUser("root");
    setPassword("secret");
    setDB("seng");
    
    _conn = NULL;
}

/**
   @brief Default destructor
   
   Disconnect from the database server and destroy the instance of 
   class.
  */
Database::~Database()
{
    LOG_DTOR();
    disconnect();
}

/**
   @brief Connect to database after object is created.
 
   If you call this method on an object that is already connected to 
   a database server, the previous connection is dropped and a new 
   connection is established.
 
   @return    True if connection was estabilished successfully.
 */
bool Database::connect()
{    
    if (_conn)
        disconnect();
    
    try {
        _conn = new Connection(false);
        _conn->set_option(new MultiStatementsOption(true));
        _conn->connect(_db.c_str(), 0, _user.c_str(), _passwd.c_str());        
    }
    catch (std::exception &e) {
        cerr << "unable to connect to database ("
             << _conn->error() << endl;
        delete _conn;
        
        return false;
    }
    
    return _conn->connected();
}

/**
   @brief Drop the connection to the database server. 
 */
void Database::disconnect()
{
    if (_conn) {
        _conn->disconnect();
        delete _conn;
        _conn = NULL;
    }
}

/**
   @brief Return a connection to the database
 
   @return An instance of mysqlpp::Connection
 */
Connection *Database::getConnection() 
{ 
    return _conn; 
}

/**
   @brief Change the server we going to connect to.
 
   If we're already connected to the server, connection is dropped: a 
   new call to connect() need to be called.
 
   @param[in]    aValue The new server to connect to
   @see connect(), disconnect()
 */
void Database::setServer(string aValue)
{
    if (_server != aValue) {
        _server = aValue;
        
        if (isConnected())
            disconnect();
    }
}

/**
   @brief Change the username we are using to connect to the database 
          server.
 
   If we're already connected to the server, connection is dropped: a 
   new call to connect() need to be called.
 
   @param[in]    aValue The new username
   @see connect(), disconnect()
 */
void Database::setUser(string aValue)
{
    if (_user != aValue) {
        _user = aValue;
        
        if (isConnected())
            disconnect();        
    }
}

/**
   @brief Change the password we are using to connect to the database 
          server.
 
   If we're already connected to the server, connection is dropped: a 
   new call to connect() need to be called.
 
   @param[in]    aValue The new password
   @see connect(), disconnect()
 */
void Database::setPassword(string aValue)
{
    if (_passwd != aValue) {
        _passwd = aValue;
        
        if (isConnected())
            disconnect();
    }    
}

/**
   @brief Database selection
 
   Change to a different database managed by the database server 
   we are connected to. 
 */
void Database::setDB(string aValue)
{
    if (_db != aValue) {
        _db = aValue;
        
        if (isConnected()) {
            _conn->select_db(_db);
        }        
    }
}

/**
   @brief Test connection status against the database
 
   @return    True if connection was established successfully.
 */
bool Database::isConnected() 
{
    return ((_conn) && (_conn->connected()));
}

/**
   @brief Print a generic row to stdout
 
   Print to standard out the content of the given row. A vector with
   all column widths need to be specified.
 
   @param[in]    widths Vector of column widths
   @param[in]    row The row to be printed
 */
void Database::printRow(IntVector & widths, Row & row)
{
    cout << "  |" << setfill(' ');
    for (size_t i = 0; i < row.size(); ++i) {
        cout << " " << setw(widths.at(i)) << row[int(i)] << " |";
    }
    cout << endl;
}

/**
   @brief Print a result set to stdou
 
   Print to standard out the content of the given result set. 
   A vector with all column widths is built and passed to printRow().
 
   @param[in]    res The result set
   @see    printRow()
 */
void Database::printResult(StoreQueryResult& res)
{
    StoreQueryResult::size_type num_results = res.size();
    if (!res || (num_results == 0)) {
        return;
    }
    
    IntVector widths;
    size_t size = res.num_fields();
    for (size_t i = 0; i < size; i++) {
        widths.push_back(max(res.field(i).max_length(), 
                             res.field_name(i).size()));
    }

    for (StoreQueryResult::size_type i = 0; i < num_results; ++i) {
        printRow(widths, res[i]);
    }
}

ostream& operator<<(ostream& aStream, Database& d) {
    return  aStream << "Connection to database is" << 
    (d._conn->connected()?"":" NOT") << " established\n";
}

