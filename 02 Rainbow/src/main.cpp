/**
 * @file main.cpp
 * @brief Main entry of the application.
 */
#include "generateRT/generateRT.h"
#include "crack/crackRT.h"
#include "util/passwd-utils.hpp"
#include "util/rt-utils.hpp"
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <sstream>
#include <fstream>

using namespace be::esi::secl::pn;

const std::string PWD_FILE("rsc/pwdToCrack.txt");           /**< The file which will contain some passwords. This file is not necessary to crack */
const std::string HASH_FILE("rsc/hashToCrack.txt");         /**< The file which will contain the passwords hashes to crack */
const std::string CRACKED_PWD_FILE("rsc/crackedPwd.txt");   /**< The file which will contain the cracked passwords */
const std::string CRACKED_HASH_FILE("rsc/crackedHash.txt"); /**< The file which will contain the cracked passwords hashes */

sqlite3 *db; /**< The database for the rainbow table */

/**
 * Entry point of the application
 * It will create and open a sqlite DB, and set it up with a configuration that maximizes speed. Note that
 * with some configuration, the database can be corrupted in the event of a crash or power outage.
 * After that, it will generate the RT with default (no param) or user values (first param is the number of head to create, second is the 
 * number of reduce to apply, the third the lenght of the password).
 * Finally, it will crack the table.
 * @param argc The number of params. Can be 1 or 4.
 * @param argv The params. Can be empty (except the name of the app). If not empty, param at position 1, 2 and 3 must be positive numericals values.
 * The first is the number of head to create, the second the number of reduce to apply on the head to get the tail, and the third the lenght og the password.
 * If one value is not correct, the behavior is undetermined.
 */
int main(int argc, char *argv[])
{
    //Open DB
    if (sqlite3_open(DB_NAME.c_str(), &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    //Set up DB
    sqlite3_exec(db, "PRAGMA synchronous = OFF", 0, 0, 0);   // Don’t Sync to Disk After Every Insert. WARNING: could cause a database corruption in the event of a crash or power outage
    sqlite3_exec(db, "PRAGMA journal_mode = OFF", 0, 0, 0);  // Disable Rollback Journal. WARNING: can loose some data in case of error
    sqlite3_exec(db, "PRAGMA cache_size = 100000", 0, 0, 0); // Increase cache size to hold the transaction
    sqlite3_exec(db, "PRAGMA page_size = 16384", 0, 0, 0);   // Increase page size

    if (argc == 1) // With default values
    {
        std::cout << "Generating the table..." << std::endl;
        generateRT(db);
        std::cout << "Cracking the table..." << std::endl;
        rainbow::mass_generate(100, PWD_SIZE, PWD_SIZE, PWD_FILE, HASH_FILE); // Generate passwords and hashes to crack. Only the hash file is used
        crack(HASH_FILE, db, CRACKED_PWD_FILE, CRACKED_HASH_FILE);
    }
    else if (argc == 4) // With user values
    {
        unsigned nbHead, pwdSize;
        int nbReduce;

        // Get the params
        nbHead = strtoul(argv[1], NULL, 10);
        nbReduce = atoi(argv[2]);
        pwdSize = strtoul(argv[3], NULL, 10);

        std::cout << "Success rate expected with the params : "
                  << getPercentage(nbHead, nbReduce, pwdSize, SIZE_AZ_O9)
                  << '\n'
                  << "Generating the table..."
                  << std::endl;
        generateRT(db, nbHead, nbReduce, pwdSize);
        std::cout << "Cracking the table..." << std::endl;
        rainbow::mass_generate(100, pwdSize, pwdSize, PWD_FILE, HASH_FILE); // Generate passwords and hashes to crack. Only the hash file is used
        crack(HASH_FILE, db, CRACKED_PWD_FILE, CRACKED_HASH_FILE, pwdSize, nbReduce);
    }

    //Check success rate
    std::cout << "Success : " << rainbow::mass_check(CRACKED_PWD_FILE, CRACKED_HASH_FILE) << std::endl;

    // Close DB
    sqlite3_close(db);
}