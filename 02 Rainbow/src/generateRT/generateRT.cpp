/**
 * @file generateRT.cpp
 * @brief Definition of function of generateRT.h
 */
#include "../util/passwd-utils.hpp"
#include "../util/rt-utils.hpp"
#include "generateRT.h"
#include <fstream>
#include <algorithm>
#include <thread>

namespace be::esi::secl::pn
{

inline const unsigned NB_THREADS_GENERATE = 10; /**< Number of thread to create to generate the RT */

void generateRT(sqlite3 *db, unsigned nbHead, int nbReduce)
{
    sqlite3_exec(db, DROP_RT, 0, 0, 0);
    if (sqlite3_exec(db, CREATE_RT, 0, 0, 0) != SQLITE_OK)
    {
        throw std::runtime_error("Can't create the table in DB!");
    }

    std::vector<std::thread> threads;
    for (unsigned i = 0; i < NB_THREADS_GENERATE; i++)
    {
        threads.push_back(std::thread(generateRTInThread, db, nbHead / NB_THREADS_GENERATE, nbReduce));
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });
}

void generateRTInThread(sqlite3 *db, unsigned nbHead, int nbReduce)
{
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, INSERT_RT, -1, &stmt, 0);

    std::string passwd, reduced, hash;

    for (unsigned i = 1; i <= (nbHead / 1000); ++i)
    {
        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

        for (unsigned j = 0; j < 1000; j++)
        {
            passwd = rainbow::generate_passwd(MAX_PWD_SIZE);
            hash = getHash(passwd);
            int idxReduction = 0;
            while (idxReduction < 5882)
            {
                reduced = reduce(hash, idxReduction++, 7);
                hash = getHash(reduced);
            }
            while (idxReduction < 6000)
            {
                reduced = reduce(hash, idxReduction++, 6);
                hash = getHash(reduced);
            }

            sqlite3_bind_text(stmt, 1, passwd.c_str(), passwd.length(), SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, reduced.c_str(), reduced.length(), SQLITE_STATIC);
            sqlite3_step(stmt);
            // if (sqlite3_step(stmt) != SQLITE_DONE) //Can take a lot of time when a lot of collisions appends
            //     i--; 

            sqlite3_clear_bindings(stmt);
            sqlite3_reset(stmt);
        }

        sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
    }
}

} //NAMESPACE be::esi::secl::pn