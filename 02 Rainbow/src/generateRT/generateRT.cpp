#include "generateRT.h"
#include "../util/sha256.h"
#include "../util/passwd-utils.hpp"
#include <fstream>
#include <iostream>

namespace be::esi::secl::pn
{

std::string reduce(const std::string &hash, int idxReduction)
{
    std::string pwd(MAX_PWD_SIZE, 'A'); //Fill pwd with fake values

    for (int i = MAX_PWD_SIZE - 1; i >= 0; i--) //TODO generate pwd with length btw 6 and 8
    {
        pwd[i] = AZ_O9[(ID_AZ_O9[hash.at(i)] + (idxReduction % SIZE_AZ_O9)) % SIZE_AZ_O9];
        idxReduction /= SIZE_AZ_O9;
    }

    return pwd;
}

std::string getHash(const std::string &input)
{
    return sha256(input);
}

void generateRT(sqlite3 *db, unsigned nbReduce)
{
    sqlite3_exec(db, DROP_RT, 0, 0, 0);
    if (sqlite3_exec(db, CREATE_RT, 0, 0, 0) != SQLITE_OK)
    {
        throw std::runtime_error("Can't create the table in DB!");
    }
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, INSERT_RT, -1, &stmt, 0);

    std::string passwd, reduced, hash;

    //TODO for debugging purposes
    std::ofstream hashesOutput("rsc/hashes.txt");
    if(!hashesOutput.is_open()){
        std::cerr << "Couldn't open hashes output file!" << std::endl;
    }

    for (int i = 0; i < NB_PASSWD; i++)
    {
        passwd = rainbow::generate_passwd(MAX_PWD_SIZE);
        hash = getHash(passwd);
        hashesOutput << hash << "\n";
        for (unsigned j = 0; j < nbReduce; j++)
        {
            reduced = reduce(hash, j);
            hash = getHash(reduced);
        }
        sqlite3_bind_text(stmt, 1, passwd.c_str(), passwd.length(), SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, reduced.c_str(), reduced.length(), SQLITE_STATIC);
        sqlite3_step(stmt);
        //TODO: check if error (not if not success)
    }

    hashesOutput.close();
}

} //NAMESPACE be::esi::secl::pn