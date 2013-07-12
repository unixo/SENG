/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: User.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __USER_H__
#define __USER_H__

#include "common.h"
#include "ManagedObject.h"
#include "Basket.h"
#include "Order.h"

using namespace std;
using namespace mysqlpp;

/**
   User is the base class to permit login into the system and benefit
   of all services, such as product browsing, placing an order and 
   so on.
   It represents instances of the table "users" but it cannot be 
   instantiated directly as it's an abstract class (see methods 
   getBasket and placeOrder).
 
   @note Class destructor needs to be virtual so that will be called 
         when a derived class is used.
 
   @see getBasket, placeOrder
 */
class User : public ManagedObject
{
public:
    User();
    User(Row & aRow);
    virtual ~User();
    
    static User * factory(string aName, string aSurname, string aLogin, 
                          string aPasswd, string anAddress = "", 
                          string aCity = "");
    static User * userByID(int anUid);
    static User * login(string username, string passwd);

    virtual Basket * getBasket() = 0;
    virtual Order * placeOrder() throw (string) = 0;

    ulonglong uniqueID();
    string fullName();
    string loginName();
    bool isAdmin();
    string primaryKey();
    
    friend ostream& operator<<(ostream &, User &);
};


/**
   This class is derived from User and is associated to the entity
   customer of the ER model. An administrator should be considered an
   instance of a generic user but with higher privileges, such that he 
   can administer the system.
   As derived from User, the class must of course implements all virtual 
   pure methods; anyway, the operations of placing an order or getting 
   the product basket have no meaning for an administrator: for this 
   reason, these methods return a null value, which must be handled by
   the client programmer.
 */
class AdminUser : public User
{
public:
    AdminUser(Row &aRow);
    Basket * getBasket() { return NULL; };
    Order * placeOrder() throw (string) { return NULL; };
    vector<User *> & userList();
    bool changeUserPassword(User & anUser, string aPasswd);
    void showMonthlyTrend();
    bool deleteProduct(int aPid);
};

/**
   Even this class, as far as AdminUser, derives from User and is 
   associated with entity customer, detailed in ER model. A normal 
   user must be intended as a customer who need to browse the product 
   catalog and, possibly, buy one or more products.
   Whenever a registered user wants to buy a product, he must choose 
   how many pieces of each product should be added to the basket and 
   ask the system to place a new order: the class Basket, as described 
   further on, is the container of products the user chose; it appears 
   clear that the implementation of virtual method getBasket() will 
   return the instance of class Basket owned by the current user.
 */
class NormalUser : public User
{
private:
    Basket basket;
    
public:
    NormalUser();
    NormalUser(Row &aRow);
    ~NormalUser();
    Order * placeOrder() throw (string);
    Basket * getBasket() { return &basket; };
};

#endif /* __USER_H__ */
