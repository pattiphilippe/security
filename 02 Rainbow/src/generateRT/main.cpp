/**
 * @file main.cpp
 * @brief Main entry for to generate the Rainbow Table
 */
#include "generateRT.h"
#include "../util/rt-utils.hpp"
#include <iostream>
#include <sqlite3.h>

using namespace be::esi::secl::pn;

/**
 * Entry point to generate the Rainbow Table.
 * It will create and open a sqlite DB, and set it up with a configuration that maximizes speed. Note that
 * with some configuration, the database can be corrupted in the event of a crash or power outage.
 * After that, it will generate the RT with default (ne param) or user values (first param is the number of head to create, second is the 
 * number of reduce to apply).
 * 
 * @param argc The number of params
 * @param argv The params. Can be empty to use default values. If not empty, param at position 1, 2 and 3 must be positive numerical values.
 * The first is the number of head to create, the second the number of reduce to apply on the head to get the tail, and the 
 * third is the passwords size.
 * If the values are not valid, behavior is undetermined.
 */
int main(int argc, char *argv[])
{
    //Open DB
    sqlite3 *db;
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

    //Get params
    unsigned pwdSize, nbHead, nbReduce;

    if (argc == 1) //With default values
    {
        pwdSize = PWD_SIZE;
        nbHead = NB_HEAD;
        nbReduce = NB_REDUCE;
    }
    else if (argc == 4) // With user values
    {
        nbHead = strtoul(argv[1], NULL, 10);
        nbReduce = strtoul(argv[2], NULL, 10);
        pwdSize = strtoul(argv[3], NULL, 10);
    }
    else
    {
        std::cerr << "Usage is without parameters or with 3 parameters : "
                  << "(number of heads) (number of reduce) (password size)" << std::endl;
        return -1;
    }

    std::cout << "Success rate expected with"
              << " number of heads = " << nbHead
              << ", number of reduce = " << nbReduce
              << ", password's size = " << pwdSize
              << " : "
              << getPercentage(nbHead, nbReduce, pwdSize, SIZE_AZ_O9)
              << '\n'
              << "Generating the table..."
              << std::endl;

    generateRT(db, pwdSize, nbHead, nbReduce);

    // Close DB
    sqlite3_close(db);
}
