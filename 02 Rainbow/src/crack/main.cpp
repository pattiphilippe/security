/**
 * @file main.cpp
 * @brief Main entry for the crack
 */
#include "crackRT.h"
#include "../util/passwd-utils.hpp"
#include "../generateRT/generateRT.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sqlite3.h>
#include "../util/sha256.h"
#include "../util/rt-utils.hpp"

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
    //rainbow::mass_generate(100, 5, 5, PWD_FILE, HASH_FILE);

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

    // std::cout << "getPercentage(2204, 50000, 5, 36) : " << getPercentage(30000, 50000, 5, 36) << std::endl;

    // std::string pwd = rainbow::generate_passwd(8);
    // std::cout << "pwd " << pwd << std::endl;
    // unsigned char digest[32], digest2[32];

    // SHA256 ctx = SHA256();
    // sha256(ctx, pwd, digest);

    // std::string hashHex = sha256ToHex(digest);
    // std::cout << "hashHex1 : "
    //           << hashHex << std::endl;

    // sha256ToDec(hashHex, digest2);
    // std::string hashHex2 = sha256ToHex(digest2);
    // std::cout << "hashHex2 : "
    //           << hashHex2 << std::endl;

    // std::cout << "pwd test 2 " << pwd << std::endl;
    // sha256(ctx, pwd, digest);

    // hashHex = sha256ToHex(digest);
    // std::cout << "hashHex1 : "
    //           << hashHex << std::endl;

    // sha256ToDec(hashHex, digest2);
    // hashHex2 = sha256ToHex(digest2);
    // std::cout << "hashHex2 : "
    //           << hashHex2 << std::endl;
}