/**
 * @file generateRT.cpp
 * @brief Definition of function of generateRT.h
 */
#include "../util/passwd-utils.hpp"
#include "../util/rt-utils.hpp"
#include "../util/sha256.h"
#include "generateRT.h"
#include <fstream>
#include <algorithm>
#include <thread>
#include <string.h>
#include <mutex>

namespace be::esi::secl::pn
{
    std::mutex mtx;
    constexpr unsigned NB_THREADS_GENERATE = 10; /**< Number of thread to create to generate the RT */

    void generateRT(sqlite3 *db, unsigned nbHead, int nbReduce)
    {
        sqlite3_exec(db, DROP_RT, 0, 0, 0);
        if (sqlite3_exec(db, CREATE_RT, 0, 0, 0) != SQLITE_OK)
        {
            throw std::runtime_error("Can't create the table in DB!");
        }

        std::ofstream outHash("rsc/hashToCrack.txt", std::ios_base::out | std::ios_base::trunc); //TODO remove
        std::ofstream outPwd("rsc/pwdToCrack.txt", std::ios_base::out | std::ios_base::trunc);
        std::vector<std::thread> threads;
        for (unsigned i = 0; i < NB_THREADS_GENERATE; i++)
        {
            threads.push_back(std::thread(generateRTInThread, std::reference_wrapper<std::ofstream>(outPwd), std::reference_wrapper<std::ofstream>(outHash), db, nbHead / NB_THREADS_GENERATE, nbReduce));
        }

        std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });
    }

    void generateRTInThread(std::ofstream &outPwd, std::ofstream &outHash, sqlite3 *db, unsigned nbHead, int nbReduce)
    {
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, INSERT_RT, -1, &stmt, 0);

        std::string passwd, reduced(PWD_SIZE, 'A'), hash;
        int idxReduction, rc;
        unsigned char digest[SHA256::DIGEST_SIZE];
        unsigned cpt, red_by;
        SHA256 ctx = SHA256();

        for (unsigned i = 1; i <= nbHead; ++i)
        {
            passwd = rainbow::generate_passwd(rainbow::random(PWD_SIZE, PWD_SIZE));
            sha256(ctx, passwd, digest);
            hash = sha256ToHex(digest);
            mtx.lock();
            outPwd << passwd << '\n';
            outHash << hash << '\n';
            mtx.unlock();
            red_by = 0;
            REDUCE(reduced, digest, red_by, cpt);

            for (idxReduction = 1; idxReduction < nbReduce; ++idxReduction)
            {
                red_by = idxReduction;
                SHA256_REDUCE(ctx, reduced, digest, red_by, cpt);
            }

            sqlite3_clear_bindings(stmt);
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, passwd.c_str(), passwd.length(), SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, reduced.c_str(), reduced.length(), SQLITE_STATIC);
            rc = sqlite3_step(stmt);
            // if (rc != SQLITE_DONE)
            //     i--;
        }
    }

} //NAMESPACE be::esi::secl::pn