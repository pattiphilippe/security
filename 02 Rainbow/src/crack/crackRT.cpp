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

    std::mutex mtxReadHead;        /**< Mutex to read the head file with concurency */
    std::mutex mtxPrintCracked;    /**< Mutex to write cracked password and hashes with concurency */
    const unsigned NB_THREADS = 1; /**< How many threads to run to crack */

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
        std::string hash, pwd;
        int idxReduction;
        std::string tail;
        bool isCollision;
        SHA256 ctx = SHA256();

        sqlite3_stmt *stmtReadTail, *stmtReadHead;
        sqlite3_prepare_v2(db, SELECT_TAIL, -1, &stmtReadTail, 0);
        sqlite3_prepare_v2(db, SELECT_HEAD, -1, &stmtReadHead, 0);

        mtxReadHead.lock();
        std::getline(hashesInput, hash);
        mtxReadHead.unlock();

        while (hashesInput) // For each hash
        {
            idxReduction = NB_REDUCE;
            int nbCollisions = 0;
            do
            {
                tail = getTail(ctx, hash, stmtReadTail, --idxReduction); //Find line

                if (!tail.empty())
                {
                    pwd = getHead(stmtReadHead, tail);
                    findPwd(ctx, pwd, idxReduction);
                    isCollision = sha256(ctx, pwd) != hash; //TODO check if can optimize
                    std::cout << std::boolalpha << "isCollision : " << isCollision << std::endl;
                    nbCollisions++;
                }
            } while (!tail.empty() && isCollision && idxReduction > 0);
            std::cout << "nbCollisions for 1 pwd : " << nbCollisions << std::endl;
            if (!tail.empty() && !isCollision)
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

    std::string getTail(SHA256 &ctx, const std::string &hash, sqlite3_stmt *stmtReadTail, int &idxReduction)
    {
        int rc, i;
        std::string pwd(PWD_SIZE, 'A');
        unsigned char digest[SHA256::DIGEST_SIZE], hash_dec[SHA256::DIGEST_SIZE]; //TODO make static tables?
        unsigned red_by, cpt;
        sha256ToDec(hash, hash_dec);

        do
        {
            red_by = idxReduction;
            REDUCE(pwd, hash_dec, red_by, cpt);
            for (i = idxReduction + 1; i < NB_REDUCE; i++)
            {
                red_by = i;
                SHA256_REDUCE(ctx, pwd, digest, red_by, cpt);
            }

            sqlite3_clear_bindings(stmtReadTail);
            sqlite3_reset(stmtReadTail);
            sqlite3_bind_text(stmtReadTail, 1, pwd.c_str(), pwd.length(), SQLITE_STATIC);
        } while ((rc = sqlite3_step(stmtReadTail)) != SQLITE_ROW && 0 < idxReduction--);

        if (rc == SQLITE_ROW)
        {
            std::cout << "return getTail() = : " << pwd << ", idxReduction : " << idxReduction << std::endl;
            return pwd;
        }
        std::cout << "return getTail() = empty string " << std::endl;
        return std::string("");
    }

    std::string getHead(sqlite3_stmt *stmtGetHead, std::string tail)
    {
        sqlite3_clear_bindings(stmtGetHead);
        sqlite3_reset(stmtGetHead);
        sqlite3_bind_text(stmtGetHead, 1, tail.c_str(), tail.length(), SQLITE_STATIC);
        if (sqlite3_step(stmtGetHead) != SQLITE_ROW)
        {
            throw std::runtime_error("Can't get the head of the tail");
        }
        std::string head(reinterpret_cast<const char *>(sqlite3_column_text(stmtGetHead, 0)));
        return head;
    }

    void findPwd(SHA256 &ctx, std::string &pwd, int idxReduction)
    {
        unsigned red_by, cpt;
        unsigned char digest[SHA256::DIGEST_SIZE];
        for (int i = 0; i < idxReduction; i++)
        {
            red_by = i;
            SHA256_REDUCE(ctx, pwd, digest, red_by, cpt);
        }
    }

} //NAMESPACE be::esi::secl::pn