#include "generateRT.h"
#include "../util/sha256.h"
#include "../util/passwd-utils.hpp"
#include <fstream>

namespace be::esi::secl::pn
{

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

void generateRT(sqlite3 *db, int nbReduce)
{
    sqlite3_exec(db, DROP_RT, 0, 0, 0);
    if (sqlite3_exec(db, CREATE_RT, 0, 0, 0) != SQLITE_OK)
    {
        throw std::runtime_error("Can't create the table in DB!");
    }
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, INSERT_RT, -1, &stmt, 0);

    std::string passwd, reduced, hash;

    for (int i = 0; i < NB_PASSWD; i++)
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
        if (sqlite3_step(stmt) != SQLITE_DONE)
            i--;

        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
    }
}

} //NAMESPACE be::esi::secl::pn