/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Basket.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __BASKET_H__
#define __BASKET_H__

#include "common.h"

// forward declaration
class Product;
class ProductProxy;

/**
   The class Basket represents the basket of the customer who 
   wants to buy one or more products: it's intended to be used 
   by User class only.
 */
class Basket : public std::map<int, int> 
{
private:
    float _tot;
    void _realAddProduct(int pid, int qty);
    
public:
    Basket();
    ~Basket();
    
    bool addProduct(Product *p, int aQty = 1);
    bool addProduct(ProductProxy *p, int aQty = 1);
    float total() const;
    void empty();
    void removeProduct(Product *p, int aQty = 1);
    int itemCount();

    friend std::ostream & operator<<(std::ostream &, Basket &);
};

#endif /* __BASKET_H__ */
