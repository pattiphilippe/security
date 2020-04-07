#include "generateRT.h"
//TODO paths for windows and linux... Bug
// #include "..\util\sha256.h"
// #include "..\util\passwd-utils.hpp"
#include "../util/sha256.h"
#include "../util/passwd-utils.hpp"
#include <fstream>
#include <iostream>

namespace be::esi::secl::pn
{

void generatePasswords(const std::string &passwordsFile, const std::string &hashFile, unsigned nb)
{
    rainbow::mass_generate(nb, MIN_PWD_SIZE, MAX_PWD_SIZE, passwordsFile, hashFile);
}

std::string reduce(const std::string hash)
{
    static int nbReduction = 1;
    int temp = nbReduction;
    //TODO ask nbReduction in param
    std::cout << "hash : " << hash << std::endl;
    std::string pwd(MAX_PWD_SIZE, 'A');
    std::cout << "pwd : " << pwd << std::endl;

    for (int i = MAX_PWD_SIZE -1; i >= 0; i--)
    {
        /*
        pwd[i] = hash.at(i);
        char indexActualChar = indexAlphanumeric(hash.at(i));
        int move = nbReductionTemp % SIZE_ALPHA_NUM;
        int newIndex = (indexActualChar + move) % SIZE_ALPHA_NUM;
        pwd[i] = TAB_ALPHA_NUM[newIndex];
        */
        std::cout << "ID_AZ_O9[hash.at(i)] : " << ID_AZ_O9[hash.at(i)] << std::endl;
        std::cout << "temp % SIZE_AZ_O9 : " << temp % SIZE_AZ_O9 << std::endl;
        std::cout << "(ID_AZ_O9[hash.at(i)] + (temp % SIZE_AZ_O9)) : " << (ID_AZ_O9[hash.at(i)] + (temp % SIZE_AZ_O9)) << std::endl;
        int j = (ID_AZ_O9[hash.at(i)] + (temp % SIZE_AZ_O9)) % SIZE_AZ_O9;
        std::cout << "(ID_AZ_O9[hash.at(i)] + (temp % SIZE_AZ_O9)) % SIZE_AZ_O9 : " << j << std::endl;
        std::cout << "AZ_O9[j] : " << AZ_O9[j] << std::endl;
        pwd[i] = AZ_O9[(ID_AZ_O9[hash.at(i)] + (temp % SIZE_AZ_O9)) % SIZE_AZ_O9];
        temp /= SIZE_AZ_O9;
    }

    std::cout << "pwd : " << pwd << std::endl;

    nbReduction++;
    return pwd;

    /*
    pwd = "764ddsjd"
    add = 38
    for(int i = 7; i >= 0; i--) {
        pwd[i] = tab[ ( index(pwd[i]) + (add%36) ) % 36 ]
        add /= 36
    }
    */
}

int indexAlphanumeric(const char c)
{
    //TODO use a more performant method (hashmap, calculation on base of ascii codes, ... anything)
    for (int i = 0; i < SIZE_AZ_O9; i++)
        if (AZ_O9[i] == c)
            return c;
    throw std::runtime_error("Not an alphanumeric!");
}

std::string hash(const std::string &input)
{
    return sha256(input);
}

void generateTails(const std::string &hashFile, const std::string &tailsFile, unsigned nb)
{

    std::ifstream hashesInput(hashFile);
    std::ofstream tailsOutput(tailsFile);

    if (!hashesInput.is_open())
        throw std::runtime_error("Hashes file can't be opened");
    if (!tailsOutput.is_open())
        throw std::runtime_error("Tails file can't be opened");

    std::string reduced;
    std::string hash;

    while (std::getline(hashesInput, hash)) //For each hash
    {
        for (unsigned i = 0; i < nb; i++) //TODO reset 1 to nb
        {
            reduced = reduce(hash);
            hash = sha256(reduced);
        }
        tailsOutput << reduced << '\n';
    }

    hashesInput.close();
    tailsOutput.close();
}

} //NAMESPACE be::esi::secl::pn