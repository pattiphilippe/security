/**
 * @file main.cpp
 * @brief Main entry for to generate the Rainbow Table
 */
#include "generateRT.h"
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <sstream>
#include <fstream>
#include "../util/rt-utils.hpp"

using namespace be::esi::secl::pn;

/**
 * Entry point to generate the Rainbow Table.
 * It will create and open a sqlite DB, and set it up with a configuration that maximizes speed. Note that
 * with some configuration, the database can be corrupted in the event of a crash or power outage.
 * After that, it will generate the RT with default (ne param) or user values (first param is the number of head to create, second is the 
 * number of reduce to apply).
 * @param argc The number of params
 * @param argv The params. Can be empty. If not empty, param at position 1 and 2 must be numericals values.
 * The first is the number of head to create, the second the number of reduce to apply on the head to get the tail.
 * If the value is not correct, behavior is undetermined.
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

    //Generate passwords and tails and put them into the DB
    if (argc == 1)
    {
        std::cout << "Generating the table..." << std::endl;
        generateRT(db); // With default values
    }
    else if (argc == 4)
    {
        // With user values
        unsigned nbHead;
        std::stringstream strValue;
        strValue << argv[1];
        strValue >> nbHead;

        unsigned nbReduce;
        strValue << argv[2];
        strValue >> nbReduce;

        strValue << argv[3];
        strValue >> PWD_SIZE;

        generateRT(db, nbHead, nbReduce);
    }

    // Close DB
    sqlite3_close(db);

    // std::cout << "getPercentage(5000, 50000, 5, 36) : " << getPercentage(5000, 50000, 5, 36) << std::endl;
    // std::cout << "getPercentage(10000, 50000, 5, 36) : " << getPercentage(10000, 50000, 5, 36) << std::endl;
    // std::cout << "getPercentage(20000, 50000, 5, 36) : " << getPercentage(20000, 50000, 5, 36) << std::endl;
    // std::cout << "getPercentage(50000, 50000, 5, 36) : " << getPercentage(50000, 50000, 5, 36) << std::endl;
}
