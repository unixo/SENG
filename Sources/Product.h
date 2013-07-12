/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Product.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "common.h"
#include "ManagedObject.h"
#include "Category.h"

#define KEY_PRD_PID             "pid"
#define KEY_PRD_CID             "cid"
#define KEY_PRD_NAME            "name"
#define KEY_PRD_DESCR           "descr"
#define KEY_PRD_PRICE           "price"
#define KEY_PRD_AVAILABILITY    "availability"
#define KEY_PRD_DELETED         "deleted"

using namespace std;
using namespace mysqlpp;

// forward declaration
class ProductProxy;

/**
   The class Product represents a generic item to be sold. As 
   the ER model states, each product logical belongs to a category,
   i.e. a family of products of the same nature, such as monitor, 
   accessories and so on.
   The class Product of course need to implements virtual methods 
   of base class ManagedObject, its use is similar to previous 
   classes described so far: the methods factory() allows to create 
   a new instance of product, as well as productByID() fetches a 
   product from the persistent store based on the specified criteria
   and catalog() returns the entire list of available products.
*/
class Product : public ManagedObject
{
public:
    Product();
    Product(Row & aRow);
    ~Product();
    
    static Product * factory(string aName, int aCid, float aPrice, 
                              string aDescr = "empty", int aQty = 0, 
                             bool isDel = false);
    static Product * productByID(int aPid);
    static void showCompatibleProducts(int aPid);
    
    auto_ptr<Category> getCategory();
    float getPrice();
    int getAvailability();
    string primaryKey();
    
    friend ostream& operator<<(ostream &, Product &);
};


/**
   The class ProductProxy is a virtual proxy of class Product.
   There could be situation when only a proxy of the real product 
   is needed and not all its data are requested: for example, think 
   at new order placing, where only product ID is asked to be known.
   Whenever other information are requested, ProductProxy provides 
   lazy creation of the product and forwards the request.
 
   @see Product, getProduct()
 */
class ProductProxy
{
private:
    Product *_theProduct;
    int _pid;
    
protected:
    Product *getProduct() throw (InvalidArgument);
    
public:
    ProductProxy(const ProductProxy & pp);
    ProductProxy(int aPid);
    ~ProductProxy();
    
    static vector<ProductProxy *> & catalog(int aCid = 0);

    auto_ptr<Category> getCategory();
    int uniqueID() const;
    float getPrice();
    string getName();
    string getDescr();
    int getAvailability();
    bool isValid();
    
    ProductProxy & operator=(const ProductProxy&);
    friend ostream& operator<<(ostream &, ProductProxy &);
};


#endif /* __PRODUCT_H__ */
