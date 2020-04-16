#include "generateRT.h"
#include <iostream>
#include <string>
#include <sqlite3.h>

const std::string PASSWORD_FILE("rsc/pwd.txt");
const std::string HASH_FILE("rsc/hashes.txt");
const std::string TAILS_FILE("rsc/tails.txt");

using namespace be::esi::secl::pn;

int main(int argc, char *argv[])
{

    sqlite3 *db; //Open DB
    if (sqlite3_open("rsc/rt_6.sqlite", &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    //Generate passwords and tails and put them into the db
    if (argc == 1)
    {
        generateRT(db);
    } else
    {
        unsigned nbHead;
        std::stringstream strValue;
        strValue << argv[1];
        strValue >> nbHead;

        generateRT(db, nbHead);
    }
    
    sqlite3_close(db);
}