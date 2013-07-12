/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Category.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Category.h"
#include "Database.h"

#define KEY_CAT_CID         "cid"
#define KEY_CAT_NAME        "name"
#define SQL_CATEGORY_BYID   "SELECT cid, name FROM categories WHERE cid = "
#define SQL_CATEGORY_CAT    "SELECT cid, name FROM categories ORDER BY cid"

/**
   @brief Class constructor
 */
Category::Category() : ManagedObject("categories")
{
    LOG_CTOR();
}

/**
   @brief Fetch a category by specifing its ID
 
   @param[in]    aCid Category ID to be fetched
   @return    An instance of category
 */
Category *Category::categoryByID(int aCid)
{
    Category *cat = NULL;
    
    // get an instance of the database
    Database &db = Database::instance();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = db.getConnection()->query();
    q << SQL_CATEGORY_BYID << aCid;
    StoreQueryResult res = q.store();
    if (!res.empty()) {
        cat = new Category();
        cat->setIntForKey(KEY_CAT_CID, aCid);
        cat->setValueForKey(KEY_CAT_NAME, (string) res[0][KEY_CAT_NAME]);
        
        return cat;
    }

    return NULL;
}

/**
   @brief Create a new category
 
   Create a new category to be stored into the database; the method 
   store need to be called to make changes persistent.
   
   @param    aValue    The category name
   @return    An instance of the new category
 */
Category *Category::factory(string aValue)
{
    Category *newCategory = new Category();
    newCategory->setIntForKey(KEY_CAT_CID, 0);
    newCategory->setValueForKey(KEY_CAT_NAME, aValue);
    
    return newCategory;
}

/**
   @brief Returns the primary for entity "categories" (KEY_CAT_CID)
 
   @return The primary key of the entity
 */
string Category::primaryKey()
{
    return KEY_CAT_CID;
}

/**
   @brief Returns the list of available categories.
 
   @return    Vector of pointer to Category
 */
vector<Category *> &Category::catalog()
{
    // get an instance of the database
    Database& db = Database::instance();
    vector<Category *> *catalog = NULL;
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query(SQL_CATEGORY_CAT);
        StoreQueryResult res = q.store();
        
        if (!res.empty()) {
            catalog = new vector<Category *>;
            catalog->reserve(res.num_rows());
            
            for (size_t i = 0; i < res.num_rows(); ++i) {
                Category *c = new Category();
                c->setValueForKey(KEY_CAT_CID, 
                                 (string) res[i][KEY_CAT_CID]);
                c->setValueForKey(KEY_CAT_NAME, 
                                  (string) res[i][KEY_CAT_NAME]);
                catalog->push_back(c);
            }
        }                
    }
    catch (const mysqlpp::BadQuery& e) {
        // Something went wrong with the SQL query.
        cerr << "Query failed: " << e.what() << endl;
    }
    catch (const Exception& er) {
        cerr << "Error: " << er.what() << endl;
    }    
    
    return *catalog;
}

/**
   @brief Returns category name
 
   @return A string representing the category
 */
string Category::getName()
{
    return valueForKey(KEY_CAT_NAME);
}

ostream& operator<<(ostream& aStream, Category & c) {    
    return aStream << right << setfill(' ') << setw(4) << 
           c.valueForKey(KEY_CAT_CID) << " | " << left << setw(30) <<
           c.valueForKey(KEY_CAT_NAME);
}
