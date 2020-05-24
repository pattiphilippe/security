/**
 * @file main.cpp
 * @brief Main entry for the crack
 */
#include "crackRT.h"
#include "../util/passwd-utils.hpp"
#include "../generateRT/generateRT.h"
#include "../util/rt-utils.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <sqlite3.h>

const std::string PWD_FILE("rsc/pwdToCrack.txt");           /**< The file which will contain some passwords. This file is not necessary to crack */
const std::string HASH_FILE("rsc/hashToCrack.txt");         /**< The file which will contain the passwords hashes to crack */
const std::string CRACKED_PWD_FILE("rsc/crackedPwd.txt");   /**< The file which will contain the cracked passwords */
const std::string CRACKED_HASH_FILE("rsc/crackedHash.txt"); /**< The file which will contain the cracked passwords hashes */
constexpr unsigned NB_PWD_TO_GENERATE = 100;                /**< The number of passwords to generate for the crack */

using namespace be::esi::secl::pn;

/**
 * Entry for the crack.
 * It will generate some passwords (+ hashes), and crack them with the rainbow table.
 * The cracked passwords and hashes are put into a new file. This is needed to keep the passwords
 * and hashes sorted, due to the use of threads.
 * 
 * @param argc The number of params
 * @param argv The params. Can be empty to use default values. If not empty, param at position 1 and 2 must be numericals values.
 * The first is the number of reduce to apply on the head to get the tail, the second the password's size.
 * If the value is not correct, behavior is undetermined.
 */
int main(int argc, char *argv[])
{

    unsigned pwdSize, nbReduce;

    if (argc == 1) //With default values
    {
        pwdSize = PWD_SIZE;
        nbReduce = NB_REDUCE;
    }
    else if (argc == 3) // With user values
    {
        nbReduce = strtoul(argv[1], NULL, 10);
        pwdSize = strtoul(argv[2], NULL, 10);
    }
    else
    {
        std::cerr << "Usage is without parameters or with 2 parameters : "
                  << "(number of reduce) (password size)" << std::endl;
        return -1;
    }

    //Generate passwords and hashes to crack
    rainbow::mass_generate(NB_PWD_TO_GENERATE, pwdSize, pwdSize, PWD_FILE, HASH_FILE);

    //Open DB
    sqlite3 *db;
    if (sqlite3_open(DB_NAME.c_str(), &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }
    sqlite3_exec(db, "PRAGMA page_size = 16384", 0, 0, 0); // Increase page size

    // Launch the crack and print the success rate
    std::cout << "Cracking the table..." << std::endl;
    crack(HASH_FILE, db, CRACKED_PWD_FILE, CRACKED_HASH_FILE, pwdSize, nbReduce);
    std::cout << "Success : " << rainbow::mass_check(CRACKED_PWD_FILE, CRACKED_HASH_FILE) / NB_PWD_TO_GENERATE << std::endl;
    
    // Close DB
    sqlite3_close(db);
}