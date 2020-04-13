#include "crackRT.h"
#include "../util/passwd-utils.hpp"
#include <string>
#include <iostream>
#include <sqlite3.h>

const std::string HASH_FILE("rsc/hashes.txt");
const std::string CRACKED_PWD_FILE("rsc/crackedPwd.txt");
const std::string CRACKED_HASH_FILE("rsc/crackedHash.txt");

using namespace be::esi::secl::pn;

int main()
{
    sqlite3 *db; //Open DB
    if (sqlite3_open("rsc/rt.sqlite", &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    crack(HASH_FILE, db, CRACKED_PWD_FILE, CRACKED_HASH_FILE);
    std::cout << "Success : " << rainbow::mass_check(CRACKED_PWD_FILE, CRACKED_HASH_FILE) << std::endl;
}