#include "generateRT.h"
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

std::string reduce(const std::string &hash, int idxReduction)
{
    std::string pwd(MAX_PWD_SIZE, 'A'); //Fill pwd with fake values

    for (int i = MAX_PWD_SIZE -1; i >= 0; i--) //TODO generate pwd with length btw 6 and 8
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
        for (unsigned i = 0; i < nb; i++)
        {
            reduced = reduce(hash, i);
            hash = getHash(reduced);
        }
        tailsOutput << reduced << '\n';
    }

    hashesInput.close();
    tailsOutput.close();
}

} //NAMESPACE be::esi::secl::pn