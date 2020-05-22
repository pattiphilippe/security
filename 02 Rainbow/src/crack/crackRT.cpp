/**
 * @file crackRT.cpp
 * @brief Declaration of functions of crackRT.h
 */
#include "crackRT.h"
#include "../util/rt-utils.hpp"
#include "../generateRT/generateRT.h"
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string.h>

namespace be::esi::secl::pn
{

    std::mutex mtxReadHead;             /**< Mutex to read the head file with concurency */
    std::mutex mtxPrintCracked;         /**< Mutex to write cracked password and hashes with concurency */
    constexpr unsigned NB_THREADS = 10; /**< How many threads to run to crack */

    void crack(const std::string &hashFile, sqlite3 *db, const std::string &crackedPwdFile, const std::string &crackedHashFile)
    {

        std::ifstream hashesInput(hashFile);
        std::ofstream crackedPwdOutput(crackedPwdFile);
        std::ofstream crackedHashOutput(crackedHashFile);

        if (!hashesInput.is_open())
            throw std::runtime_error("Hashes file can't be opened");
        if (!crackedPwdOutput.is_open())
            throw std::runtime_error("Cracked passwords file can't be opened");
        if (!crackedHashOutput.is_open())
            throw std::runtime_error("Cracked passwords hashes file can't be opened");

        std::vector<std::thread> threads;
        for (unsigned i = 0; i < NB_THREADS; i++)
        {
            threads.push_back(std::thread(crackInThread, std::ref(hashesInput), db,
                                          std::ref(crackedPwdOutput), std::ref(crackedHashOutput))); //Need to wrap the references
        }

        std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });

        hashesInput.close();
        crackedPwdOutput.close();
        crackedHashOutput.close();
    }

    void crackInThread(std::ifstream &hashesInput, sqlite3 *db, std::ofstream &crackedPwdOutput, std::ofstream &crackedHashOutput)
    {

        sqlite3_stmt *stmtReadTail, *stmtReadHead;
        sqlite3_prepare_v2(db, SELECT_TAIL, -1, &stmtReadTail, 0);
        sqlite3_prepare_v2(db, SELECT_HEAD, -1, &stmtReadHead, 0);

        std::string hash(SHA256::DIGEST_SIZE * 2, 'A'), pwd(PWD_SIZE, 'A');
        unsigned char hash_dec[SHA256::DIGEST_SIZE], digest[SHA256::DIGEST_SIZE];
        SHA256 ctx = SHA256();

        mtxReadHead.lock();
        std::getline(hashesInput, hash);
        mtxReadHead.unlock();

        while (hashesInput) // For each hash
        {

            if (getPwd(hash, pwd, stmtReadTail, stmtReadHead, hash_dec, digest, ctx))
            {
                mtxPrintCracked.lock();
                crackedPwdOutput << pwd << std::endl;   //Write found pwd
                crackedHashOutput << hash << std::endl; //Write hash
                mtxPrintCracked.unlock();
            }
            else
            {
                mtxPrintCracked.lock();
                crackedPwdOutput << std::endl;          //Write unfound pwd
                crackedHashOutput << hash << std::endl; //Write hash
                mtxPrintCracked.unlock();
            }

            mtxReadHead.lock();
            std::getline(hashesInput, hash);
            mtxReadHead.unlock();
        }
    }

    bool getPwd(const std::string &hash, std::string &pwd, sqlite3_stmt *stmtReadTail, sqlite3_stmt *stmtReadHead,
                unsigned char hash_dec[], unsigned char digest[], SHA256 &ctx)
    {
        int idxReduction = NB_REDUCE - 1, rc, i;
        unsigned red_by, cptPwdSize;

        sha256ToDec(hash, hash_dec);

        for (; 0 <= idxReduction; --idxReduction)
        {
            red_by = idxReduction;
            REDUCE(pwd, hash_dec, red_by, cptPwdSize);
            i = idxReduction + 1;
            SHA256_REDUCE_X_TIMES(ctx, pwd, digest, NB_REDUCE, red_by, i, cptPwdSize)

            sqlite3_clear_bindings(stmtReadTail);
            sqlite3_reset(stmtReadTail);
            sqlite3_bind_text(stmtReadTail, 1, pwd.c_str(), pwd.length(), SQLITE_STATIC);
            rc = sqlite3_step(stmtReadTail);

            if (rc == SQLITE_ROW)
            {
                GET_HEAD(stmtReadHead, pwd)
                i = 0;
                SHA256_REDUCE_X_TIMES(ctx, pwd, digest, idxReduction, red_by, i, cptPwdSize) //Find pwd
                SHA256_(ctx, pwd, digest)
                if (std::equal(digest, digest + SHA256::DIGEST_SIZE, hash_dec))
                    return true;
            }
        }
        return false;
    }

} //NAMESPACE be::esi::secl::pn