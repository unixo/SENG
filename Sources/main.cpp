/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: main.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "Database.h"
#include "UserMenu.h"
#include "CommandLine.h"

int debugLevel = 0;

int main (int argc, char * const argv[]) 
{
    // parse command line
    CommandLine cmd(argc, argv);
    if (cmd.isFault())
        return 1;
    
    // set debug level
    debugLevel = cmd.debugLevel();
    
    // get an instance of the database
    Database& db = Database::instance();
    db.setServer(cmd.dbServer());
    db.setUser(cmd.dbUser());
    db.setPassword(cmd.dbPasswd());
    
    // try to connect to database: halt the program if not
    if (!db.connect()) {
        cerr << "Unable to connect to database\n";
        return 2;
    }

    // display main menu
    UserMenu menu;
    menu.mainMenu();
    
    return 0;
}
