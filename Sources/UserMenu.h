/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: UserMenu.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __USERMENU_H__
#define __USERMENU_H__

#include "common.h"
#include "User.h"
#include "Exceptions.h"

class UserMenu;

typedef void (UserMenu::*op)();

/**
   @brief Display menus and handles user input
 */
class UserMenu 
{
private:        
    User *_currentUser;
    map<int, op> usr_operations;
    map<int, op> adm_operations;
    
    void printCatalog(int aCid = 0);
    
    // user operations
    void browseProductCatalog() throw (BadAuthException);
    void showProductDetail() throw (BadAuthException);
    void showConfigurationByProduct() throw (BadAuthException);
    void showUserProfile() throw (BadAuthException);
    void placeNewOrder() throw (BadAuthException);
    
    // admin operations    
    void addNewCategory() throw (BadAuthException);
    void addNewProduct() throw (BadAuthException);
    void changeProductDetail() throw (BadAuthException);
    void disableUser() throw (BadAuthException);
    void displayMonthlyTrend() throw (BadAuthException);
    void deleteProduct() throw (BadAuthException);
    
    void displayUnprivilegedMenu() throw (BadAuthException);
    void displayAdminMenu() throw (BadAuthException);
    void wait();
    
    bool getNotEmptyLine(const string & msg, string *buffer);

protected:
    void enableSttyEcho(bool bValue = true);

public:
    UserMenu();
    ~UserMenu();
    
    void mainMenu();
    bool login();
    void display() throw (BadAuthException);
    void registerNewUser();
};

#endif /* __USERMENU_H__ */
