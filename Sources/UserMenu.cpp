/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: UserMenu.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "UserMenu.h"
#include "Product.h"
#include "Order.h"
#include <termios.h>
#include <algorithm>

/**
   @brief Default constructor
 */
UserMenu::UserMenu()
{
    LOG_CTOR();
    _currentUser = NULL;
    
    usr_operations[0] = &UserMenu::browseProductCatalog;
    usr_operations[1] = &UserMenu::showProductDetail;
    usr_operations[2] = &UserMenu::showConfigurationByProduct;
    usr_operations[3] = &UserMenu::showUserProfile;
    usr_operations[4] = &UserMenu::placeNewOrder;
    
    adm_operations[0] = &UserMenu::addNewCategory;
    adm_operations[1] = &UserMenu::addNewProduct;
    adm_operations[2] = &UserMenu::changeProductDetail;
    adm_operations[3] = &UserMenu::disableUser;
    adm_operations[4] = &UserMenu::displayMonthlyTrend;
    adm_operations[5] = &UserMenu::deleteProduct;
    
    LOG(3, "#%d usr operations loaded\n", (int)usr_operations.size());
    LOG(3, "#%d adm operations loaded\n", (int)adm_operations.size());
}

/**
   @brief Default destructor
 
   Dealloc the logged user, if any.
 */
UserMenu::~UserMenu()
{
    LOG_DTOR();
    if (_currentUser)
        delete _currentUser;
}

/**
   @brief Wait for user to press enter/return to continue.
 */
void UserMenu::wait()
{
    cin.clear();
    cout << "\nPress Enter/Return to continue...\n";
    cin.ignore(1, 0); 
}

/**
   @brief Get input from standard input
 
   @param[in]    msg    String to prompt before getting input
   @param[out]    buffer Pointer to string to fill with data
   @return    True if operation was successful and buffer has been filled
 */
bool UserMenu::getNotEmptyLine(const string & msg, string *buffer)
{    
    string tmp;
    
    //cin.clear();        
    cout << msg;
    do {
        getline(cin, tmp);
    } while (tmp.size() == 0);
    
    *buffer = string(tmp);
    
    return true;
}

/**
   @brief Display user menu (for not-admin users)
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::displayUnprivilegedMenu() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    int choice;
    
    do {
        system(CLEAR_SCREEN_CMD);
        cout << "USER MENU\n" <<
        "(1)  Browse product catalog by category\n"
        "(2)  View details of a chosen product\n"
        "(3)  Browse all the configurations which include a given item\n"
        "(4)  Browse user profile and all his order\n"
        "(5)  Place a new order\n"
        "(0)  LOGOUT\n\n"
        "Current user: " << _currentUser->fullName() <<
        "\n\nMake your choice: ";
        
        cin >> choice;
        if (choice > 0 && choice <= (int) usr_operations.size()) {
            int idx = choice - 1;
            op anOP = usr_operations[idx];
            (this->*anOP)();
        }
    } while (choice != 0);
    
    delete _currentUser, _currentUser = NULL;
}

/**
   @brief Browse product catalog by category
 
   Display all products belonging to a category or display them 
   all if any category is chosen.
 
   @exception BadAuthException If called without being authenticated
 */
void UserMenu::browseProductCatalog() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    int cid;
    
    system(CLEAR_SCREEN_CMD);
    
    // list all available category and ask the user to choose one of them
    cout << "BROWSE PRODUCT CATALOG\n\nCATEGORY LIST\n";
    vector<Category *> & vc = Category::catalog();
    if (&vc && vc.size()) {
        for (int i=0; i < (int)vc.size(); i++) {
            cout << *(vc[i]) << endl;
        }
    }
    std::for_each(vc.begin(), vc.end(), deletePtr<Category>());
    cout << "\nEnter category ID [0 to browse all]: ";
    cin >> cid;
    cout << endl;
    printCatalog(cid);    
    wait();
}

/**
   @brief Display all product beloging to a category ID
 
   @param[in]    aCid    Category ID
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::printCatalog(int aCid)
{
    // list all product belonging to selected category (or display them all if
    // zero was selected.
    vector<ProductProxy *> & v = ProductProxy::catalog(aCid);
    if (&v && v.size()) {
        for (int i=0; i < (int)v.size(); i++) {
            ProductProxy *pp = v[i];
            auto_ptr<Category> c = pp->getCategory();
            
            cout << "  |" << setfill(' ') << setw(5) << right << pp->uniqueID() 
                 << " |" << left << setw(20) << c->getName() << "|  "
                 << setw(35) << pp->getName() << "|  " 
                 << setw(7) << right << pp->getPrice() << endl;
        }
        cout << endl;
        
        // dealloc container
        std::for_each(v.begin(), v.end(), deletePtr<ProductProxy>());
        delete &v;    
    }    
}

/**
   @brief Display all details of a given product
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::showProductDetail() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    int pid;
    
    system(CLEAR_SCREEN_CMD);
    cout << "SHOW PRODUCT DETAIL\n\nProduct catalog\n";
    printCatalog();
    
    cout << "\nEnter product ID: ";
    cin >> pid;
    
    try {
        ProductProxy pp(pid);
        if (pp.isValid())
            cout << endl << pp;            
    }
    catch (exception & e) {
        cout << "\nInvalid product ID\n";
    }
    
    wait();
}

/**
   @brief Show all product configurations which include a given product
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::showConfigurationByProduct() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    int pid;
    
    system(CLEAR_SCREEN_CMD);
    cout << "BROWSE ALL CONFIGURATIONS WHICH INCLUDE A GIVEN ITEM\n\n"
    "Choose a product from the following list\n\n";
    
    printCatalog();
    
    cout << "\nEnter product ID: ";
    cin >> pid;    
    
    Product::showCompatibleProducts(pid);
    wait();
}

/**
   @brief Show user profile and all his orders.
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::showUserProfile() throw (BadAuthException)
{        
    if (!_currentUser)
        throw BadAuthException();
    
    system(CLEAR_SCREEN_CMD);
    cout << "USER DETAILS\n" << *_currentUser << endl << endl;
    
    // obtain a vector containing all orders for current logged user
    vector<Order *> & orders = Order::ordersForUser(*_currentUser);
    
    if (orders.size()) {        
        // we don't need to update this vector: 
        // let's use a const_iterator (safe)
        vector<Order *>::const_iterator it;
        
        // iterate on all vector items and print them
        for (it = orders.begin(); it != orders.end(); it++) {
            Order *ord = (Order *) *it;
            cout << *ord << endl;
            
            map<int, int> &products = ord->products();
            map<int,int>::const_iterator mit;
            
            for (mit = products.begin(); mit != products.end(); mit++) {
                ProductProxy pp = ProductProxy( (*mit).first );
                cout << "  (*) Product: " << left << setw(30) << pp.getName()  
                << "Quantity: " << (*mit).second << endl;
            }
            cout << endl << endl;
            delete &products;
        }
        
        // dealloc container
        std::for_each(orders.begin(), orders.end(), deletePtr<Order>());        
    } else {
        cout << "[INFO] User didn't place any order at the moment.\n";
    }
    
    delete &orders;    
    
    wait();
}

/**
   @brief Place a new order
 
   Help the user to choose one or more items from available 
   products and add them to his basket; after last confirmation, 
   a new order is created.
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::placeNewOrder() throw (BadAuthException)
{    
    if (!_currentUser)
        throw BadAuthException();
    
    bool addMoreProducts = true;
    
    do {
        int aPid, aQty;
        
        system(CLEAR_SCREEN_CMD);
        cout << "CREATE ORDER\n\n";
        printCatalog();
        cout << "#" << (int)_currentUser->getBasket()->itemCount() 
             << " item(s) in basket\n\nEnter product ID to add [0 to end]: ";
        cin >> aPid;
        if (aPid == 0)
            addMoreProducts = false;
        else {
            cout << "How many pieces: ";
            cin >> aQty;
            
            Basket *bkt = _currentUser->getBasket();
			try {
				if (! bkt->addProduct(new ProductProxy(aPid), aQty)) {
					cerr << "\nUnable to add requested item to basket.\n";
					wait();
				}				
			}
			catch (const exception & er) {
				cerr << "\n\nError: product not existent\n";
				wait();
			}    
        }
    } while (addMoreProducts);
    
    if (_currentUser->getBasket()->itemCount() == 0) {
        cout << "No products were added to basket.\n";
        wait();
        return;
    }
    cout << "\nSUMMARY\n=======\n" << *(_currentUser->getBasket()) << endl
         << "TOTAL: " << _currentUser->getBasket()->total() << endl;
    _currentUser->placeOrder();
    wait();
}

/**
   @brief Display all available admin operations.
 
   @throw BadAuthException if called without the correct level 
          of authorization.
 */
void UserMenu::displayAdminMenu() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    int choice;
    
    do {
        system(CLEAR_SCREEN_CMD);
        cout << "ADMINISTRATION MENU\n" <<
        "(1)  Add a new category\n"
        "(2)  Add a new product\n"
        "(3)  Change product details\n"
        "(4)  Disable an user\n"
        "(5)  Display monthly trend\n"
        "(6)  Delete a product\n"
        "(0)  EXIT\n\n"
        "Current admin: " << _currentUser->fullName() <<
        "\n\nMake your choice: ";
        cin >> choice;
        
        if (choice > 0 && choice <= (int) adm_operations.size()) {
            int idx = choice - 1;
            op anOP = adm_operations[idx];
            (this->*anOP)();
        }
    } while (choice != 0);
}

/**
   @brief Admin operation to create a new category of products.
 
   @exception BadAuthException If called without being authenticated
 */
void UserMenu::addNewCategory() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    Category *newCat;
    string name;
    bool bValue;
    
    system(CLEAR_SCREEN_CMD);
    cout << "ADD A NEW CATEGORY\n\n";
    bValue = getNotEmptyLine("Enter the name of new category [0 to quit]: ", 
                             &name);
    if (!bValue || name == "0")
        cerr << "Operation aborted.\n";
    else {
        newCat = Category::factory(name);
        newCat->store();
        cout << "\nOperation was successful.\n";
    }
    wait();
}

/**
   @brief Admin operation to display monthly trend of sales
 
   @exception BadAuthException If called without being authenticated 
              or not authorized
 */
void UserMenu::displayMonthlyTrend() throw (BadAuthException)
{
    AdminUser *anAdmin = dynamic_cast<AdminUser *> (_currentUser);
    
    if (!_currentUser || !anAdmin)
        throw BadAuthException();
    
    system(CLEAR_SCREEN_CMD);
    cout << "MONTHLY TREND\n\n  | YEAR | MONTH |       TOTAL |\n"
         << "--------------------------------\n";
    anAdmin->showMonthlyTrend();
    wait();
}

/**
   @brief Admin operation to add a new product.
 
   @exception BadAuthException If called without being authenticated
 */
void UserMenu::addNewProduct() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    string name, descr = "", cidStr, priceStr, qtyStr;
    float price;
    int qty, cid;
    bool success;
    
    system(CLEAR_SCREEN_CMD);
    cout << "ADD NEW PRODUCT\n\nChoose the category of new product from list\n";    
    
    vector<Category *> & v = Category::catalog();
    if (&v && v.size()) {
        for (int i=0; i < (int)v.size(); i++) {
            cout << *(v[i]) << endl;
        }
    } else {
        cerr << "There are no categories avabilable." << endl
             << "Create a new category, then add a product." << endl;
        wait();
        
        return;
    }        
    
    // we can dealloc container, categories are only printed
    std::for_each(v.begin(), v.end(), deletePtr<Category>());
    delete &v;
    
    success = getNotEmptyLine("\nEnter category ID [0 to abort]: ", &cidStr);
    if (!success || cidStr == "0") {
        cerr << "\nOperation aborted.\n";
        wait();
        return;
    }
    getNotEmptyLine("Product name                  : ", &name);
    getNotEmptyLine("Product description           : ", &descr);
    getNotEmptyLine("Price                         : ", &priceStr);
    getNotEmptyLine("Availability                  : ", &qtyStr);
    
    price = atof(priceStr.c_str());
    qty = atoi(qtyStr.c_str());
    cid = atoi(cidStr.c_str());
    
    if ((cid <= 0) || (price <= 0.0)) {
        cerr << "\nOperation aborted.\n";
    } else {
        Product *p = Product::factory(name, cid, price, descr, qty);
        p->store();    
        delete p;
    }
    
    wait();
}

/**
   @brief Admin operation to update an existing product.
 
   @exception BadAuthException If called without being authenticated
 */
void UserMenu::changeProductDetail() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    Product *p;
    int pid, attr;
    string attribute;
    
    // display the product catalog
    printCatalog();
    
    // identify the product to edit or abort the operation
    cout << "Enter product ID to change [0 to abort]: ";
    cin >> pid;
    if (pid == 0) {
        cout << "\nOperation aborted.\n";
        wait();
        return;
    }
    
    // retrive the product from database
    p = Product::productByID(pid);
    if (!p) {
        cerr << "[ERR] Unable to find specified product. Abort.\n";
        wait();
        return;
    }
    cout << "\nREVIEW PRODUCT DETAIL\n " << *p << endl;
    
    do {
        cout << "\nCHOOSE WHICH ATTRIBUTE YOU WANT TO EDIT:\n"
             << "[1] Name\n[2] Description\n[3] Price\n[0] End\n\n"
             << "Enter attribute ID: ";
        cin >> attr;
        if (attr != 0) {            
            if (getNotEmptyLine("Enter new value: ", &attribute)) {
                switch (attr) {
                    case 1:
                        p->setValueForKey(KEY_PRD_NAME, attribute);
                        break;
                    case 2:
                        p->setValueForKey(KEY_PRD_DESCR, attribute);
                        break;
                    case 3:
                        p->setValueForKey(KEY_PRD_PRICE, attribute);
                        break;
                    default:
                        attr = 0;
                        break;
                }            
            }            
        }        
    } while (attr != 0);
    
    if (p->update())
        cout << "Operation successfully completed\n";
    else
        cerr << "Something went wrong. Operation aborted.\n";
    
    wait();
}

/**
   @brief Admin operation to disable a registered user
 
   @exception BadAuthException If called without being authenticated 
             or not authorized
 
   @see AdminUser::changeUserPassword()
 */
void UserMenu::disableUser() throw (BadAuthException)
{
    AdminUser *anAdmin = dynamic_cast<AdminUser *> (_currentUser);
    
    if (!_currentUser || !anAdmin)
        throw BadAuthException();
    
    system(CLEAR_SCREEN_CMD);
    cout << "USER LOCK\n\n";
    
    vector<User *> & uc = anAdmin->userList();
    int pid, i;
    
    for (i=0; i<(int)uc.size(); i++) {
        User *anUser = uc[i];
        cout << setw(4) << i+1 << " | " << anUser->fullName() 
             << " (" << anUser->loginName() << ")" << endl;
    }
    cout << "\nEnter user ID to disable: ";
    cin >> pid;
    if ((pid <= 0)) {
        cerr << "\nInvalid selection\n";
        wait();
        return;
    }
    
    // Check if given user ID properly identifies an user: otherwise 
    // abort the operation.
    User *anUser = User::userByID(pid);
    if (!anUser) {
        cerr << "\nInvalid user ID entered. Operation aborted.\n";
    } else {
        bool bValue = anAdmin->changeUserPassword(*anUser, "*LK*");
        
        cout << "\nOperation was " << (bValue?"":"NOT ") 
        << "succesfully completed\n";
    }
    
    // dealloc container
    std::for_each(uc.begin(), uc.end(), deletePtr<User>());
    delete &uc;
    
    wait();
}

/**
   @brief Delete a product (even if already sold).
 
   @throw    BadAuthException If called by client programmer without 
            any logged user
 
   @see AdminUser::deleteProduct()
 */
void UserMenu::deleteProduct() throw (BadAuthException)
{
    AdminUser *anAdmin = dynamic_cast<AdminUser *> (_currentUser);
    
    if (!_currentUser || !anAdmin)
        throw BadAuthException();
    
    int pid;
    
    system(CLEAR_SCREEN_CMD);
    cout << "PRODUCT DELETE\n\n";
    
    // Display product catalog
    printCatalog();
    
    cout << "\nEnter product ID to delete [0 to abort] :";
    cin >> pid;
    if (pid <= 0) {
        cerr << "Operation aborted.\n";
    } else {
        bool b = anAdmin->deleteProduct(pid);
        if (b)
            cout << "\nProduct successfully deleted.\n";
        else
            cerr << "\nSomething went wrong. Operation aborted.\n";
    }
    wait();
}

/**
   @brief Display a menu to the screen, according to authorization 
          level of current user.
 
   @throw    BadAuthException If called by client programmer without 
            any logged user
 
   @see displayAdminMenu(), displayUnprivilegedMenu()
 */
void UserMenu::display() throw (BadAuthException)
{
    if (!_currentUser)
        throw BadAuthException();
    
    if (_currentUser->isAdmin())
        displayAdminMenu();
    else
        displayUnprivilegedMenu();
}

/**
   @brief User login
 
   Ask user to enter his crendentials and test the access: if 
   successful, an instance of User is allocated and assigned to 
   _currentUser.
 
   @return    True if user logged in successfully (credentials were 
            valid).
 
   @see User::login(), enableSttyEcho()
 */
bool UserMenu::login()
{
    string username;
    string passwd;
    
    do {
        system(CLEAR_SCREEN_CMD);
        cout << "USER LOGIN\nLogin [0 to quit]: ";
        cin >> username;
        if (username == "0") {
            cerr << "\nOperation aborted.\n";
            wait();
            return false;        
        }
        
        cout << "Password         : ";
        enableSttyEcho(false);
        cin >> passwd;
        enableSttyEcho();
        
        if ((_currentUser = User::login(username, passwd)))
            return true;
        
        cerr << "\n\nInvalid username/password.\n";
        wait();
    } while (1);
    
    return false;
}

/**
   @brief Main menu
 
   Displays the main menu: user is allowed to login into the 
   system, register himself as new user or quit the application.
 
   @see login(), registerNewUser(), display()
 */
void UserMenu::mainMenu()
{
    int choice;
    
    do {
        system(CLEAR_SCREEN_CMD);
        cout << "MAIN MENU\n\n(1) User login\n(2) New user registration\n" 
              << "(0) Quit\n\nMake your selection: ";    
        cin >> choice;
        if (choice == 1) {
            if (login())
                display();
        } else if (choice == 2)
            registerNewUser();            
    } while (choice != 0);
}

/**
   @brief Give the user the opportunity to register as new user
 
   @see User::factory
 */
void UserMenu::registerNewUser()
{
    string name, surname, login, passwd, address, city;
    bool success = false;
    
    system(CLEAR_SCREEN_CMD);
    cout << "NEW USER REGISTRATION\n\n";

    success = getNotEmptyLine("Enter name    : ", &name);
    if (success) {
        getNotEmptyLine("Enter surname : ", &surname);
        getNotEmptyLine("Enter address : ", &address);
        getNotEmptyLine("Enter city    : ", &city);
        getNotEmptyLine("Enter login   : ", &login);
        getNotEmptyLine("Enter password: ", &passwd);

        if (User::factory(name, surname, login, passwd, address, city) == NULL)
            cerr << "\nUnable to register new user."
                 << "Try to review your input data.\n";
        else
            cout << "\nOperation successfully completed.\n";
    }

    wait();
}

/**
   @brief Enable or disable STTY echo
 
   This function disables (or enables) the echo of input coming 
   from the STTY, such as the keyboard; calling this method is 
   useful when the user is asked to enter his password and this 
   data don't need to be echo-ed to screen.
 
   @see login()
 */
void UserMenu::enableSttyEcho(bool bValue)
{
    struct termios tty;
    
    tcgetattr(STDIN_FILENO, &tty);
    if (!bValue)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;
    
    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
