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

namespace be::esi::secl::pn
{

std::mutex mtxReadHead;         /**< Mutex to read the head file with concurency */
std::mutex mtxPrintCracked;     /**< Mutex to write cracked password and hashes with concurency */
const unsigned NB_THREADS = 10; /**< How many threads to run to crack */

void crack(const std::string &hashFile, sqlite3 *db, const std::string &crackedPwdFile, const std::string &crackedHashFile, unsigned pwdSize, int nbReduce)
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
                                      std::ref(crackedPwdOutput), std::ref(crackedHashOutput), pwdSize, nbReduce)); //Need to wrap the references
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });

    hashesInput.close();
    crackedPwdOutput.close();
    crackedHashOutput.close();
}

void crackInThread(std::ifstream &hashesInput, sqlite3 *db, std::ofstream &crackedPwdOutput, std::ofstream &crackedHashOutput, unsigned pwdSize, int nbReduce)
{
    std::string hash, pwd, head;
    int idxReduction;
    std::string tail;
    bool isCollision;

    sqlite3_stmt *stmtReadTail, *stmtReadHead;
    sqlite3_prepare_v2(db, SELECT_TAIL, -1, &stmtReadTail, 0);
    sqlite3_prepare_v2(db, SELECT_HEAD, -1, &stmtReadHead, 0);

    mtxReadHead.lock();
    std::getline(hashesInput, hash);
    mtxReadHead.unlock();

    while (hashesInput) // For each hash
    {
        idxReduction = nbReduce;
        do
        {
            tail = getTail(hash, stmtReadTail, --idxReduction, pwdSize, nbReduce); //Find line

            if (!tail.empty())
            {
                head = getHead(stmtReadHead, tail);
                pwd = findPwd(head, idxReduction, pwdSize);
                isCollision = getHash(pwd) != hash; //TODO check if needed
            }
        } while (!tail.empty() && isCollision);

        if (!tail.empty() && !isCollision)
        {
            mtxPrintCracked.lock();
            crackedPwdOutput << pwd << '\n';   //Write found pwd
            crackedHashOutput << hash << '\n'; //Write hash
            mtxPrintCracked.unlock();
        }
        else
        {
            mtxPrintCracked.lock();
            crackedPwdOutput << '\n';          //Write unfound pwd
            crackedHashOutput << hash << '\n'; //Write hash
            mtxPrintCracked.unlock();
        }

        mtxReadHead.lock();
        std::getline(hashesInput, hash);
        mtxReadHead.unlock();
    }
}

std::string getTail(const std::string &hash, sqlite3_stmt *stmtReadTail, int &idxReduction, unsigned pwdSize, int nbReduce)
{
    int rc, i;
    std::string reduced = reduce(hash, idxReduction, pwdSize);
    sqlite3_bind_text(stmtReadTail, 1, reduced.c_str(), reduced.length(), SQLITE_STATIC);

    while ((rc = sqlite3_step(stmtReadTail)) != SQLITE_ROW && 0 < idxReduction--)
    {
        sqlite3_clear_bindings(stmtReadTail);
        sqlite3_reset(stmtReadTail);

        //Reduce until supposed tail
        reduced = reduce(hash, idxReduction, pwdSize);
        for (i = idxReduction + 1; i < nbReduce; i++) 
            reduced = reduce(getHash(reduced), i, pwdSize);

        sqlite3_bind_text(stmtReadTail, 1, reduced.c_str(), reduced.length(), SQLITE_STATIC);
    }

    sqlite3_clear_bindings(stmtReadTail);
    sqlite3_reset(stmtReadTail);

    if (rc == SQLITE_ROW)
    {
        return reduced;
    }
    return std::string("");
}

std::string getHead(sqlite3_stmt *stmtGetHead, const std::string &tail)
{
    sqlite3_bind_text(stmtGetHead, 1, tail.c_str(), tail.length(), SQLITE_STATIC);
    if (sqlite3_step(stmtGetHead) != SQLITE_ROW)
    {
        throw std::runtime_error("Can't get the head of the tail");
    }
    std::string head((const char *)sqlite3_column_text(stmtGetHead, 0));
    sqlite3_clear_bindings(stmtGetHead);
    sqlite3_reset(stmtGetHead);
    return head;
}

std::string findPwd(std::string pwd, int idxReduction, unsigned pwdSize)
{
    for (int i = 0; i < idxReduction; i++)
        pwd = reduce(getHash(pwd), i, pwdSize);

    return pwd;
}

} //NAMESPACE be::esi::secl::pn