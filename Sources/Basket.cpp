/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Basket.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Basket.h"
#include "Product.h"

/**
   @brief Default constructor
 */
Basket::Basket()
{
    LOG_CTOR();
    _tot = 0.0;
}

/**
   @brief Default destructor
 */
Basket::~Basket()
{
    LOG_DTOR();
}

/**
   @brief Add product to basket
 
   Add a product to the basket. It's possibile to specify how many 
   pieces of the same product.
   A check against product availability is done before adding it 
   to the basket.
 
   @param[in]    p An instance to ProductProxy
   @param[in]    aQty How many pieces of the same product 
                     (default is 1)
   @return    True if successful, false otherwise
 */
bool Basket::addProduct(ProductProxy *p, int aQty)
{
    if (aQty > p->getAvailability())
        return false;
    
    _tot += p->getPrice();
    
    int aPid = p->uniqueID();
    _realAddProduct(aPid, aQty);
    
    return true;
}

/**
   @brief Add product to basket
 
   Add a product to the basket. It's possibile to specify how 
   many pieces of the same product.
   A check against product availability is done before adding 
   it to the basket.
 
   @param[in]    p An instance to Product
   @param[in]    aQty How many pieces of the same product 
                     (default is 1)
   @return    True if successful, false otherwise
 */
bool Basket::addProduct(Product *p, int aQty)
{
    if (aQty > p->getAvailability())
        return false;
    
    _tot += p->getPrice();
    
    int aPid = p->intForKey(p->primaryKey());
    _realAddProduct(aPid, aQty);
    
    return true;
}

/**
   @brief Real add product
 
   This function must not be used directly, but only called by
   addProduct() - that's why it's private. It adds the chosen 
   product and requested quantity to the map, by first looking 
   for an equal entry: in this case only quantity is increased.
 
   @param[in]    pid    Product ID to add
   @param[in]    qty    Requested quantity
 */
void Basket::_realAddProduct(int pid, int qty)
{
    int aValue = qty;
    
    map<int,int>::iterator it = find(pid);
    if (it != end())
        aValue += (*it).second;
    insert(pair<int, int>(pid, aValue));        
}

/**
   @brief Empty the basket
 */
void Basket::empty()
{
    _tot = 0.0;
    clear();
}

/**
   @brief Remove a product from the basket
 
   The function looks for the given product and decreases 
   the quantity: if this value goes down below or equal to 
   zero, the product is removed; otherwise only quantity is 
   decreased.
 
   @param[in]    p    Pointer to Product to be removed
   @param[in]    aQty    Requested quantity to remove
 */
void Basket::removeProduct(Product *p, int aQty)
{
    int aPid = p->intForKey(p->primaryKey());
    map<int,int>::iterator it = find(aPid);
    if (it != end()) {
        int aValue = (*it).second;
        aValue -= aQty;
        if (aValue <= 0)
            erase(it);
        else
            insert(pair<int, int>(aPid, aValue));
    }
}

/**
   @brief Items count
 
   Return the number of item currently present in the basket.
 
   @return Number of items
 */
int Basket::itemCount()
{
    map<int,int>::const_iterator it;
    int count = 0;
    
    for (it = begin(); it != end(); it++)
        count += (*it).second;
    
    return count;
}

/**
   @brief Return the total cost of the basket
 
   @return    The total, expressed as float
 */
float Basket::total() const
{ 
    return _tot; 
}

ostream& operator<<(ostream& aStream, Basket & p) {    
    for (map<int, int>::const_iterator it=p.begin(); it != p.end(); it++) {
        ProductProxy p = ProductProxy( (*it).first );
        aStream << left << setw(30) << p.getName() << " | " << right
                << (*it).second << endl;
    }
        
    return aStream;
}
