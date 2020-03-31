#include "generateRT.h"
#include "..\util\sha256.h"
#include "..\util\passwd-utils.hpp"
#include <fstream>

namespace be::esi::secl::pn
{

void generatePasswords(const std::string &passwordsFile, const std::string &hashFile, unsigned nb)
{
    rainbow::mass_generate(nb, MIN_PWD_SIZE, MAX_PWD_SIZE, passwordsFile, hashFile);
}

std::string reduce(const std::string hash)
{
    static int c = 0;

    std::string pwd(hash.begin(), hash.begin() + MAX_PWD_SIZE - 1);
    pwd += c + 'a';

    c = (c + 1) % 26;

    return pwd; //TODO: check if function reduce is valid
}

std::string hash(const std::string &input)
{
    return sha256(input);
}

void generateTails(const std::string &hashFile, const std::string &tailsFile, unsigned nb)
{

    std::ifstream hashesInput(hashFile);
    std::ofstream tailsOutput(tailsFile);
    std::string hash;

    if (!hashesInput.is_open())
        throw std::runtime_error("Hashes file can't be opened");
    if (!tailsOutput.is_open())
        throw std::runtime_error("Tails file can't be opened");

    while (std::getline(hashesInput, hash)) //For each hash
    {
        std::string reduced;
        std::string newHash;
        for (unsigned i = 0; i < nb; i++)
        {
            reduced = reduce(hash);
            newHash = sha256(reduced);
        }
        tailsOutput << reduced << '\n';
    }
}

} //NAMESPACE be::esi::secl::pn