/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: User.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "User.h"
#include "Basket.h"

#define KEY_USR_UID         "uid"
#define KEY_USR_NAME        "name"
#define KEY_USR_SURNAME     "surname"
#define KEY_USR_ADDRESS     "address"
#define KEY_USR_CITY        "city"
#define KEY_USR_LOGIN       "login"
#define KEY_USR_PASSWD      "password"
#define KEY_USR_ADMIN       "admin"
#define QUERY_LOGIN         "SELECT * FROM users WHERE login = %0q AND " \
                            "password = %1q"
#define QUERY_FETCH         "SELECT * FROM users WHERE uid = "
#define QUERY_ADMIN_USERLST "SELECT * FROM users WHERE admin=0 "\
                            "ORDER BY surname, name"
#define QUERY_ADMIN_TREND   "SELECT DATE_FORMAT(date, '%Y') AS year, " \
                            "DATE_FORMAT(date, '%m') as month, " \
                            "SUM(total) AS total_sales FROM orders " \
                            "GROUP BY year, month ORDER BY year, month"


/**
   @brief Default constructor
 
   Used to create a new instance to be saved later.
 
   @see factory, ManagedObject
 */
User::User() : ManagedObject("users")
{    
    LOG_CTOR()
}

/**
   @brief Class constructor
 
   Constructs an instance of User and fill it with data taken from 
   mysqlpp::Row
 
   @param    aRow Record fetched from the database
   @see ManagedObject, mysqlpp::Row
 */
User::User(Row &aRow) : ManagedObject("users", aRow)
{
    LOG_CTOR()    
}

/**
   @brief Default destructor
 */
User::~User()
{
    LOG_DTOR()    
}

/**
   @brief Returns the primary for entity "users" (KEY_USR_UID)
   
   @return The primary key of the entity
 */
string User::primaryKey()
{
    return KEY_USR_UID;
}

/**
   @brief Create a new user
 
   Create a new instance of an unprivileged user, initialized 
   with parameters.
 
   @param[in]    aName    The name of the user
   @param[in]    aSurname The surname of the user
   @param[in]    aLogin The login of the user
   @param[in]    aPasswd The password of the user
   @param[in]    anAddress The address of the user
   @param[in]    aCity The city of the user
 */
User * User::factory(string aName, string aSurname, string aLogin, 
                     string aPasswd, string anAddress, string aCity)
{
    User *nu = new NormalUser();
    nu->setIntForKey(KEY_USR_UID, 0);
    nu->setIntForKey(KEY_USR_ADMIN, 0);
    nu->setValueForKey(KEY_USR_NAME, aName);
    nu->setValueForKey(KEY_USR_SURNAME, aSurname);
    nu->setValueForKey(KEY_USR_ADDRESS, anAddress);
    nu->setValueForKey(KEY_USR_CITY, aCity);
    nu->setValueForKey(KEY_USR_LOGIN, aLogin);
    nu->setValueForKey(KEY_USR_PASSWD, aPasswd);
    if (!nu->store()) {
        delete nu;
        
        return NULL;
    }        
    
    return nu;
}

/**
   @brief User login
 
   Test given user credential to log into the system: if successful 
   an instance of NormalUser or AdminUser is returned, according to 
   authorization level, NULL otherwise.
 
   @param[in]    username The username to test
   @param[in]    passwd The password to test
   @return    An instance of User class
 */
User * User::login(string username, string passwd)
{
    // get an instance of the database
    Database &db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query(QUERY_LOGIN);
    q.parse();
    
    StoreQueryResult res = q.store(username, passwd);
    if (!res.empty()) {
        StoreQueryResult::const_iterator it = res.begin();
        Row row = *it;
        
        User *newUser = NULL;
        
        if (row[KEY_USR_ADMIN] == "1")
            newUser = new AdminUser(row);
        else
            newUser = new NormalUser(row);
                        
        return newUser;
    } 
    
    return NULL;
}

/**
   @brief User constructor by ID
 
   Fetch a user by specifing its ID: if successful, an instance of 
   NormalUser or AdminUser is returned, according to authorization 
   level, NULL otherwise.
 
   @param[in]    anUid User ID to be fetched
   @return    An instance of User
 */
User * User::userByID(int anUid)
{
    // get an instance of the database
    Database &db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query();
    q << QUERY_FETCH << anUid;    
    StoreQueryResult res = q.store();
    if (!res.empty()) {
        StoreQueryResult::const_iterator it = res.begin();
        Row row = *it;
        
        if (row[KEY_USR_ADMIN] == "1")
            return new AdminUser(row);
        else
            return new NormalUser(row);
    }
    
    return NULL;
}

/**
   @brief Returns the user ID which uniquely identifies this instance
 
   @return    The user ID
 */
ulonglong User::uniqueID()
{
    return (ulonglong) intForKey(KEY_USR_UID);
}

/**
   @brief Returns the full name of current user
 
   @return    The full name
 */
string User::fullName()
{
    return valueForKey(KEY_USR_NAME) + " " + valueForKey(KEY_USR_SURNAME);
}

/**
   @brief Returns the username used to log into the system
 
   @return    The login name
 */
string User::loginName()
{
    return valueForKey(KEY_USR_LOGIN);
}

/**
   @brief Returns the authorization level of current user.
 
   @return True if the current user has administration privileges.
 */
bool User::isAdmin()
{
    /**
       polymorphic approach
       (it's anyway possible to use getUser()->boolForKey(KEY_USR_ADMIN);
     */
    AdminUser *admin = dynamic_cast<AdminUser *> (this);
    
    return (admin != NULL);
}

ostream& operator<<(ostream& aStream, User & u) {
    return aStream << "Name   : " << u.valueForKey(KEY_USR_NAME) << " " 
            << u.valueForKey(KEY_USR_SURNAME) << endl
           << "Login  : " << u.valueForKey(KEY_USR_LOGIN) << endl
           << "Address: " << u.valueForKey(KEY_USR_ADDRESS) << " ("
           << u.valueForKey(KEY_USR_CITY) << ")\n";
}


/**
   @brief Class constructor by fetched record
 
   Constructs an instance of AdminUser and fill it with data taken from 
   mysqlpp::Row
 
   @param    aRow Record fetched from the database
   @see ManagedObject, User::User
 */
AdminUser::AdminUser(Row &aRow) : User(aRow)
{
    LOG_CTOR()
}

/**
   @brief Registered user list
 
   Returns the list of currently registered users, even if locked.
 
   @return    Vector of pointer to class User
   @see AdminUser, NormalUser, std::vector
 */
vector<User *> & AdminUser::userList()
{
    // get an instance of the database
    Database &db = Database::instance();
    vector<User *> *users = NULL;
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query(QUERY_ADMIN_USERLST);
    StoreQueryResult res = q.store();
    if (!res.empty()) {
        users = new vector<User *>;
        users->reserve(res.num_rows());

        StoreQueryResult::const_iterator it;
        for (it=res.begin(); it != res.end(); it++) {
            User *anUser;
            Row row = *it;
            
            if (row[KEY_USR_ADMIN] == "1")
                anUser = new AdminUser(row);
            else
                anUser = new NormalUser(row);
            assert(anUser);
            users->push_back(anUser);
        }
    }
    
    return *users;
}

/**
   @brief Delete a product
 
   If the product has already been sold in past orders, it's marked as 
   deleted, otherwise it's physically removed from the database.
 
   @param[in]    aPid    The product ID to delete
 
   @return    True if operation was successful
 */
bool AdminUser::deleteProduct(int aPid)
{
    assert(aPid >= 0);
    
    // get an instance of the database
    Database& db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query();
    q << "CALL product_delete(" << aPid << ")";
    
    SimpleResult r = q.execute();
    if (r == false) {
        LOG(2, "An error occurred during mysqlpp:query::execute\n"
            "ERR: %s\nQuery was: %s\n", q.error(), q.str().c_str());
        
        return false;
    }
    
    return true;
}

/**
   @brief Change user password
   Let an administrator to change a user password: this method could 
   also be used to disable a user login.
   To make this change persistent, remember to commit by calling store().
 
   @param[in]    anUser Instance of user to alter
   @param[in]    aPasswd New password to assign
   @return    True if change was successful
   @see ManagedObject::store()
 */
bool AdminUser::changeUserPassword(User & anUser, string aPasswd)
{
    assert(anUser.uniqueID() != 0);
    
    anUser.setValueForKey(KEY_USR_PASSWD, aPasswd);
    return anUser.update();
}

/**
   @brief Displays the monthly trend of sales, grouped by year/month.
 */
void AdminUser::showMonthlyTrend()
{
    // get an instance of the database
    Database &db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query(QUERY_ADMIN_TREND);
    StoreQueryResult res = q.store();
    db.printResult(res);
}



/**
   @brief Default constructor
   
   @see User, ManagedObject
 */
NormalUser::NormalUser() : User()
{    
    LOG_CTOR()
}

/**
   @brief Class constructor by fetched record
 
   Constructs an instance of NormalUser and fill it with data taken 
   from mysqlpp::Row
 
   @param    aRow Record fetched from the database
   @see ManagedObject, User::User, mysqlpp::Row
 */
NormalUser::NormalUser(Row &aRow) : User(aRow)
{
    LOG_CTOR()
}

/**
   @brief Default destructor
 */
NormalUser::~NormalUser()
{
    LOG_DTOR()
}

/**
   @brief New order creation
 
   Call this method when the user has a filled his basket and want 
   to place a new order; if successful, an instance of class Order is 
   returned and the basket is cleared.
 
   @return    An instance of class Order if successful, NULL otherwise
   @see Order, Order::create()
 */
Order * NormalUser::placeOrder() throw (string)
{
    if (basket.size() == 0)
        throw "Basket must contain at least one product";
    
    Order *newOrder = Order::create(intForKey(KEY_USR_UID), basket);
    if (newOrder) {
        basket.clear();
    }
    
    return newOrder;
}
