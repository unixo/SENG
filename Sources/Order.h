/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Order.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __ORDER_H__
#define __ORDER_H__

#include "ManagedObject.h"
#include "Basket.h"

using namespace mysqlpp;

// forward declaration
class User;

/**
   This class realizes the main target of the overall system: 
   let a user to buy products by placing an order; analyzing 
   the ER model, it’s possible to see that an order is a 
   master-detail structure, in which the master is represented 
   by this class and contains information about the owner of the 
   order, the date and the overall total; the detail part is made
   up of the list of chosen products (relation 'madeup' of ER model).
 */
class Order : public ManagedObject 
{
private:
    User *_user;
    
public:
    Order();
    Order(Row &aRow);
    ~Order();
    
    string primaryKey();
    
    static Order *create(int anUid, Basket & bsk);
    static vector<Order *> & ordersForUser(User & pp);
    map<int, int>& products();
    
    friend ostream& operator<<(ostream &, Order &);
};

#endif /* __ORDER_H__ */
