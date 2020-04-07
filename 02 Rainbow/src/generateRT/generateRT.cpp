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
    //TODO if running this method twice, nbReduction doesn't start at 0 ; ask nbReduction in param?
    int temp = nbReduction;
    std::string pwd(MAX_PWD_SIZE, 'A');

    for (int i = MAX_PWD_SIZE -1; i >= 0; i--)
    {
        pwd[i] = AZ_O9[(ID_AZ_O9[hash.at(i)] + (temp % SIZE_AZ_O9)) % SIZE_AZ_O9];
        temp /= SIZE_AZ_O9;
    }

    nbReduction++;
    return pwd;
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