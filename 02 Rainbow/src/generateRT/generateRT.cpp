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
    /*
    
    pwd = "764ddsjd"
    add = 38
    for(int i = 7; i >= 0; i--) {
        pwd[i] = tab[ ( index(pwd[i]) + (add%36) ) % 36 ]
        add /= 36
    }
    */
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
            reduced = reduce(hash);
            hash = sha256(reduced);
        }
        tailsOutput << reduced << '\n';
    }
    
    hashesInput.close();
    tailsOutput.close();
}

} //NAMESPACE be::esi::secl::pn