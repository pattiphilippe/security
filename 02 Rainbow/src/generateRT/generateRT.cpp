/**
 * @file generateRT.cpp
 * @brief Definition of function of generateRT.h
 */
#include "generateRT.h"
#include "../util/sha256.h"
#include "../util/passwd-utils.hpp"
#include <fstream>
#include <thread>

namespace be::esi::secl::pn
{

inline const unsigned NB_THREADS_GENERATE = 10; /**< Number of thread to create to generate the RT */

std::string reduce(const std::string &hash, int idxReduction)
{
    unsigned long long x = std::stoull(hash.substr(0, 10), 0, 36);
    std::string pwd(MAX_PWD_SIZE, 'A');

    for (int i = 0; i < MAX_PWD_SIZE; i++)
    {
        pwd[i] = AZ_O9[x % SIZE_AZ_O9];
        x /= SIZE_AZ_O9;
    }

    return pwd;
}

std::string getHash(const std::string &input)
{
    return sha256(input);
}

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
            for (int j = 0; j < nbReduce; j++)
            {
                reduced = reduce(hash, j);
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