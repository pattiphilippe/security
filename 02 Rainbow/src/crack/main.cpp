#include "crackRT.h"
#include "../util/passwd-utils.hpp"
#include <string>
#include <iostream>
#include <sqlite3.h>

const std::string PWD_FILE("rsc/pwdToCrack.txt");
const std::string HASH_FILE("rsc/hashToCrack.txt");
const std::string CRACKED_PWD_FILE("rsc/crackedPwd.txt");
const std::string CRACKED_HASH_FILE("rsc/crackedHash.txt");

using namespace be::esi::secl::pn;

int main()
{
    //rainbow::mass_generate(100, 4, 4, PWD_FILE, HASH_FILE);
    sqlite3 *db; //Open DB
    if (sqlite3_open("rsc/rt_6.sqlite", &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    crack(HASH_FILE, db, CRACKED_PWD_FILE, CRACKED_HASH_FILE);
    std::cout << "Success : " << rainbow::mass_check(CRACKED_PWD_FILE, CRACKED_HASH_FILE) << std::endl;
}