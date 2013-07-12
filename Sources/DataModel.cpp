/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: DataModel.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "DataModel.h"
#include "Database.h"

/**
   @brief Default constructor
 */
DataModel::DataModel()
{
    LOG_CTOR();
}

/**
   @brief Default destructor
 
   Remove all cached models
 */
DataModel::~DataModel()
{
    LOG_DTOR();
    _models.clear();
}

/**
   @brief Ask for the data model of given entity
 
   The first time the method keysForEntity() is called, the class 
   asks the database for attributes list of given entity, while
   immediately returns the same information whenever the same question is
   issued again. 
 
   @param[in]    anEntity    Entity name
   @return    A set of string representing table columns
 
   @see StringSet
 */
StringSet & DataModel::keysForEntity(string anEntity)
{
    map<std::string, StringSet *>::const_iterator it = _models.find(anEntity);
    if (it != _models.end()) {
        LOG(3, "Data model for entity '%s' found in cache.\n", anEntity.c_str());
        StringSet *ss = (*it).second;
        
        return *ss;
    }
    
    // get an instance of the database
    Database & db = Database::instance();
    StringSet *ss = new StringSet();
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query("SELECT * FROM " + anEntity);
        StoreQueryResult res = q.store();
        
        for (size_t i = 0; i < res.field_names()->size(); i++) {
            ss->insert(res.field_name(i).c_str());
        }
        
        _models.insert(pair<string, StringSet *>(anEntity, ss));
    }
    catch (const mysqlpp::BadQuery& er) {
        cerr << "Query error: " << er.what() << endl;
    }
    catch (const mysqlpp::Exception& er) {
        cerr << "Error: " << er.what() << endl;
    }
    LOG(3, "Data model for entity '%s' loaded.\n", anEntity.c_str());
    
    return *ss;
}
