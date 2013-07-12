/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: Product.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Product.h"
#include "Database.h"

#define SQL_CATALOG_PROXY        "SELECT pid FROM catalogue "
#define SQL_PRODUCT_PROXY        "SELECT * FROM products WHERE pid = "

/**
   @brief Default constructor
 */
Product::Product() : ManagedObject("products")
{
    LOG_CTOR();
}

/**
   @brief Construct a Product with values contained in a 
          mysqlpp::Row
 
   @see mysqlpp::Row
 */
Product::Product(Row & aRow) : ManagedObject("products", aRow)
{
    LOG_CTOR();
}

/**
   @brief Default destructor
 */
Product::~Product()
{
}

/**
   @brief New product creation
 
   Static method to create a new product (without saving into 
   the database): product details are filled with given parameters.
 
   @param[in] aName    Product name
   @param[in] aCid    Category ID
   @param[in] aPrice    Product price
   @param[in] aDescr    Product detailed description
   @param[in] aQty    Number of stocked items
   @param[in] isDel    True if the product is not available
 
   @return    An instance of the new product
 */
Product *Product::factory(string aName, int aCid, float aPrice, string aDescr,
                          int aQty, bool isDel)
{        
    Product *newProduct = new Product();
    newProduct->setIntForKey(KEY_PRD_PID, 0);
    newProduct->setIntForKey(KEY_PRD_CID, aCid);
    newProduct->setValueForKey(KEY_PRD_NAME, aName);
    newProduct->setValueForKey(KEY_PRD_DESCR, aDescr);
    newProduct->setFloatForKey(KEY_PRD_PRICE, aPrice);
    newProduct->setIntForKey(KEY_PRD_AVAILABILITY, aQty);
    newProduct->setBoolForKey(KEY_PRD_DELETED, isDel);
    
    return newProduct;
}

/**
   @brief Returns a product with given ID
 
   @param[in] aPid    The requested product ID
 
   @return A pointer to the requested Product, NULL if not found
 */
Product * Product::productByID(int aPid)
{
    // get an instance of the database
    Database &db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query();
    q << SQL_PRODUCT_PROXY << aPid;    
    StoreQueryResult res = q.store();
    if (!res.empty()) {
        StoreQueryResult::const_iterator it = res.begin();
        Row row = *it;
        
        return new Product(row);
    }
    
    return NULL;
}

/**
   @brief Primary key
 
   Returns the primary for entity "products" (KEY_PRD_PID)
 
   @return The primary key of the entity
 */
string Product::primaryKey()
{
    return KEY_PRD_PID;
}

/**
   @brief Return the price
 
   @return A float representing the price
 */
float Product::getPrice()
{
    return floatForKey(KEY_PRD_PRICE);
}

/**
   @brief Return stock availability
 
   @return An integer representing the availability
 */
int Product::getAvailability()
{
    return intForKey(KEY_PRD_AVAILABILITY);
}

/**
   @brief Show all configurations which include a given product
 
   @param[in] aPid    Given product ID 
 */
void Product::showCompatibleProducts(int aPid)
{
    // get an instance of the database
    Database& db = Database::instance();
    
    // ask Database for a valid connection to mySQL
    Connection *conn = db.getConnection();
    
    // obtain an instance of mysqlpp::Query and init it
    Query q = conn->query();
    q << "CALL offers_by_product(" << aPid << ")";
    StoreQueryResult res = q.store();
    
    for (int i = 1; q.more_results(); ++i) {
        cout << "\n\nCONFIGURATION DETAIL\n====================\n";        
        db.printResult(res);

        cout << "\nConfiguration includes the following products:\n"
                "=============================================\n";

        res = q.store_next();
        db.printResult(res);
        cout << endl;
        
        res = q.store_next();        
        StoreQueryResult res = q.store();
    }
}

/**
   @brief Returns product category
 
   @return    An instance of class Category which the product belongs
 */
auto_ptr<Category> Product::getCategory()
{
    return auto_ptr<Category>(Category::categoryByID(intForKey(KEY_PRD_CID)));
}

ostream& operator<<(ostream& aStream, Product & p) {    
    return aStream << "PRODUCT DETAIL\n" <<
           "Category    : " << *(p.getCategory())                  << endl <<
           "Name        : " << p.valueForKey(KEY_PRD_NAME)         << endl <<
           "Description : " << p.valueForKey(KEY_PRD_DESCR)        << endl <<
           "Price       : " << p.valueForKey(KEY_PRD_PRICE)        << endl <<
           "Availability: " << p.valueForKey(KEY_PRD_AVAILABILITY) << endl;
}


/**
   @brief Constructor with given ID
 
   Construct an instance of ProductProxy by specifying product unique ID.
 
   @param[in]    aPid Product unique ID
 */
ProductProxy::ProductProxy(int aPid)
{
    _pid = aPid;
    _theProduct = NULL;
}

/**
   @brief Default destructor
 
   Dealloc instance of real object if it was allocated.
 */
ProductProxy::~ProductProxy()
{
    if (_theProduct)
        delete _theProduct;
}

/**
   @brief Obtain a real instance of the Product
 
   Concrete creation of an instance of Product class; when 
   client programmer tries to access to some ProductProxy methods, 
   such as getPrice(), a call to this method ensures that the product 
   is instantiated only we really needed.
 
   @return A pointer to class Product
 */
Product *ProductProxy::getProduct() throw (InvalidArgument)
{
    if (!_theProduct) {
        // get an instance of the database
        Database &db = Database::instance();
        
        // ask Database for a valid connection to mySQL and obtain a Query
        Query q = db.getConnection()->query();
        
        // obtain an instance of mysqlpp::Query and init it
        q << SQL_PRODUCT_PROXY << _pid;
        StoreQueryResult res = q.store();        
        
        if (res.empty())
            throw InvalidArgument("PID");
        
        StoreQueryResult::const_iterator it = res.begin();
        Row row = *it;
        
        _theProduct = new Product(row);
    }
    
    return _theProduct;
}

/**
   @brief Returns the price of this product
 
   @return    The price of the product
   @see getProduct(), Product() 
 */
float ProductProxy::getPrice()
{
    return getProduct()->getPrice();
}

/**
   @brief Returns product name
 
   @return    THe name of the product
   @see getProduct(), Product() 
 */
string ProductProxy::getName() 
{ 
    return getProduct()->valueForKey(KEY_PRD_NAME);
}

/**
   @brief Returns product description
 
   @return    The product description
   @see getProduct(), Product()
 */
string ProductProxy::getDescr()
{
    return getProduct()->valueForKey(KEY_PRD_DESCR);
}

/**
   @brief Return the product unique ID
 
   @return Product ID
 */
int ProductProxy::uniqueID() const 
{ 
    return _pid; 
}

/**
   @brief Return the product availability
 
   @return How many pieces of this product are available
 */
int ProductProxy::getAvailability()
{
    return getProduct()->intForKey(KEY_PRD_AVAILABILITY);
}

/**
   @brief Return the list of products of a given category
 
   @param[in]    aCid    The category ID
 
   @return    A vector of ProductProxy
 
   @note Pass zero as category ID to get all products 
 */
vector<ProductProxy *> & ProductProxy::catalog(int aCid)
{
    // get an instance of the database
    Database& db = Database::instance();
    vector<ProductProxy *> *catalog = NULL;
    
    try {
        // ask Database for a valid connection to mySQL
        Connection *conn = db.getConnection();
        
        // obtain an instance of mysqlpp::Query and init it
        Query q = conn->query();
        q << SQL_CATALOG_PROXY;
        if (aCid != 0)
            q << "WHERE cid = " << aCid;
        q << " ORDER BY pid, category, name";
        StoreQueryResult res = q.store();
        
        if (!res.empty()) {
            catalog = new vector<ProductProxy *>;
            catalog->reserve(res.num_rows());
            
            for (size_t i = 0; i < res.num_rows(); ++i) {
                catalog->push_back(new ProductProxy((int) res[i][0]));
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
   @brief Checks if the instance of product is valid
 
   @return    True if the instance is valid, false otherwise
 */
bool ProductProxy::isValid()
{
    return (getProduct() != NULL);
}

/**
   @brief Assignment operator
 
   The operator checks if the given item differs from this istance 
   and, in this case, it only copies product ID.
 */
ProductProxy & ProductProxy::operator=(const ProductProxy &pp)
{
    if (this != &pp) {
        _pid = pp._pid;
        _theProduct = NULL;        
    }
    
    return *this;
}

/**
   @brief Returns product category
 
   @return    An instance of class Category which the product belongs
 */
auto_ptr<Category> ProductProxy::getCategory()
{
    return getProduct()->getCategory();
}

ostream& operator<<(ostream& aStream, ProductProxy & pp) {
    Product *p = pp.getProduct();
    return aStream << *p << endl;
}
