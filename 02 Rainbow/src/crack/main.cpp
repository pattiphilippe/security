/**
 * @file main.cpp
 * @brief Main entry for the crack
 */
#include "crackRT.h"
#include "../util/passwd-utils.hpp"
#include "../generateRT/generateRT.h"
#include <string>
#include <iostream>
#include <sqlite3.h>

const std::string PWD_FILE("rsc/pwdToCrack.txt");           /**< The file which will contain some passwords. This file is not necessary to crack */
const std::string HASH_FILE("rsc/hashToCrack.txt");         /**< The file which will contain the passwords hashes to crack */
const std::string CRACKED_PWD_FILE("rsc/crackedPwd.txt");   /**< The file which will contain the cracked passwords */
const std::string CRACKED_HASH_FILE("rsc/crackedHash.txt"); /**< The file which will contain the cracked passwords hashes */

using namespace be::esi::secl::pn;

/**
 * Entry for the crack.
 * It will generate some passwords (+ hashes), and crack them with the rainbow table.
 * The cracked passwords and hashes are put into a new file. This is needed to keep the passwords
 * and hashes sorted, due to the use of threads.
 */
int main()
{
    //Generate passwords and hashes to crack
    rainbow::mass_generate(100, 5, 5, PWD_FILE, HASH_FILE);

    //Open DB
    sqlite3 *db;
    if (sqlite3_open(DB_NAME.c_str(), &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // Launch the crack and print the success rate
    crack(HASH_FILE, db, CRACKED_PWD_FILE, CRACKED_HASH_FILE);
    std::cout << "Success : " << rainbow::mass_check(CRACKED_PWD_FILE, CRACKED_HASH_FILE) << std::endl;
}