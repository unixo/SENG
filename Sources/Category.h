/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Category.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __CATEGORY_H__
#define __CATEGORY_H__

#include "common.h"
#include "ManagedObject.h"

using namespace std;
using namespace mysqlpp;

/**
   The class Category represents a logical group of products of the 
   same nature; it’s associated with the entity category of the ER 
   model and derives directly from ManagedObject.
 
   As requirements, each product must be assigned to a category, as 
   well as category can contain zero or more than one product.
 */
class Category : public ManagedObject
{
public:
    Category();
    static Category *categoryByID(int aCid);
    static Category *factory(string aValue);
    static vector<Category *> & catalog();

    string primaryKey();
    string getName();
    
    friend ostream& operator<<(ostream &, Category &);
};

#endif /* __CATEGORY_H__ */
