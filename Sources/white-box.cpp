/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: white-box.cpp 75 2010-09-13 09:25:53Z unixo $ 
 */

#include "white-box.h"
#include "CommandLine.h"

int debugLevel = 3;

/**
   @brief Test key/value observing framework
 */
void testObserver()
{
    cout << "OBSERVER/OBSERVABLE TEST #1\n";
    
    TestObservable *master = new TestObservable();
    TestObserver *slave1 = new TestObserver("slave1");
    TestObserver *slave2 = new TestObserver("slave2");
    string str1 = "value1", str2 = "value2";
    
    master->addObserver("attr", *slave1);
    master->addObserver("attr", *slave2);
    master->setKey(str1);
    
    master->removeObserver("attr", *slave1);
    master->setKey(str2);
    
    delete master, delete slave1, delete slave2;
    
    cout << endl;
}

/**
   @brief Test DataModel and Database classes
 */
void testDataModel()
{
    cout << "DATAMODEL TEST #2\n";
    
    User *usr1 = User::login("unixo", "secret");
    User *usr2 = User::userByID(2);
    
    delete usr1, delete usr2;
}

int main (int argc, char * const argv[]) 
{
    CommandLine cmd(argc, argv);
    if (cmd.isFault())
        return 1;
        
    // get an instance of the database
    Database& db = Database::instance();
    db.setServer(cmd.dbServer());
    db.setUser(cmd.dbUser());
    db.setPassword(cmd.dbPasswd());
    if (!db.connect()) {
        cerr << "Unable to connect to database\n";
        return 2;
    }
    
    // call unit tests
    testObserver();
    testDataModel();
    
    return 0;
}
