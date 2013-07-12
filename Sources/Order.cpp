/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Order.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Order.h"
#include "User.h"
#include <ctime>

#define KEY_ORD_OID         "oid"
#define KEY_ORD_UID         "uid"
#define KEY_ORD_DATE        "date"
#define KEY_ORD_TOTAL       "total"

/**
   @brief Default constructor
 */
Order::Order() : ManagedObject("orders")
{
    LOG_CTOR();
    _user = NULL;
}

/**
   @brief Construct an instance of Order with data fetched 
          from the database
 
   @param[in] aRow An instance of mysqlpp::Row with data
 */
Order::Order(Row &aRow) : ManagedObject("orders", aRow)
{    
    LOG_CTOR();
    _user = User::userByID(intForKey(KEY_ORD_UID));
}

/**
   @brief Default destructor
 
   If the user is linked to an user, dealloc the instance of class User.
 */
Order::~Order()
{
    LOG_DTOR();
    if (_user)
        delete _user;
}

/**
   @brief Create a new order
 
   @param[in] anUid    ID of user that places the order
   @param[in] bsk User basket containing products
 
   @return A pointer to an instance of Order if successful
 */
Order * Order::create(int anUid, Basket & bsk)
{
    Order *o = new Order();
    
    // get current date and format as expected by mySQL
    time_t now = time(NULL);
    struct tm *tmNow = gmtime((const time_t *)&now);
    
    stringstream nowStr;
    nowStr << (1900+tmNow->tm_year) << "-" << tmNow->tm_mon << "-" 
           << setfill('0') << setw(2) << tmNow->tm_mday << " " 
           << tmNow->tm_hour << ":" << tmNow->tm_min  
           << ":" << tmNow->tm_sec;
        
    // set other attributes
    o->setIntForKey(KEY_ORD_OID, 0);
    o->setFloatForKey(KEY_ORD_TOTAL, bsk.total());
    o->setValueForKey(KEY_ORD_DATE, nowStr.str());
    o->setIntForKey(KEY_ORD_UID, anUid);
    if (!o->store()) {
        delete o;
        LOG(2, "Unable to place the order.\n");
        
        return NULL;
    }
    
    ulonglong oid = o->getLastInsertID();
    
    // get an instance of the database
    Database& db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query();
    
    // add to "order_details" all items present in the basket
    for (Basket::const_iterator it=bsk.begin(); it != bsk.end(); it++) {
        q << "INSERT INTO order_details VALUES (" << oid << ", " 
          << (*it).first << ", " << (*it).second << ")";
        q.exec();
        q.reset();
    }

    return o;
}

/**
   @brief Return the list of all orders of a given user
 
   @param[in] pp    An instance of User
 
   @return    A vector of Order
 */
vector<Order *> & Order::ordersForUser(User & pp)
{
    vector<Order *> *orders = new vector<Order *>;
    
    // get an instance of the database
    Database& db = Database::instance();
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query();
        q << "SELECT * FROM orders WHERE uid = " << pp.uniqueID()
          << " ORDER BY oid, date";
        
        StoreQueryResult res = q.store();
        if (!res.empty()) {
            orders->reserve(res.num_rows());
            StoreQueryResult::const_iterator it;

            for (it = res.begin(); it != res.end(); it++){
                Row row = *it;
                orders->push_back(new Order(row));
            }
        }
    } catch (std::exception &e) {
        cerr << "an error occurred: " << e.what() << endl;
    }
    
    return *orders;
}

/**
   @brief Returns the list of products (and requested quantity) of an order
 
   @return    A std::map where the key is product ID and the value the quantity
 */
map<int, int>& Order::products()
{
    map<int, int> *prd = NULL;
    
    // get an instance of the database
    Database& db = Database::instance();
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query();
        q << "SELECT * FROM order_details WHERE oid = "
        << valueForKey(KEY_ORD_OID);            
        
        StoreQueryResult res = q.store();
        if (!res.empty()) {
            prd = new map<int, int>;
            StoreQueryResult::const_iterator it;
            
            for (it = res.begin(); it != res.end(); it++){
                Row row = *it;
                int key = atoi(row["pid"]);
                int qty = atoi(row["qty"]);
                (*prd)[key] = qty;
            }
        }
    } catch (std::exception &e) {
        cerr << "an error occurred: " << e.what() << endl;
    }
    
    return *prd;
}

/**
   @brief Returns the primary for entity "orders" (KEY_ORD_OID)
 
   @return The primary key of the entity
 */
string Order::primaryKey()
{
    return KEY_ORD_OID;
}

ostream& operator<<(ostream& aStream, Order & o) {    
    return aStream << "ORDER DETAIL\n============\n" <<
    "Buyer : " << o._user->fullName() << endl <<
    "Date  : " << o.valueForKey(KEY_ORD_DATE) << endl <<
    "Total : " << o.valueForKey(KEY_ORD_TOTAL) << endl;
}
