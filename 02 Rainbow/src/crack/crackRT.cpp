#include "crackRT.h"
#include <fstream>
#include <iostream>

namespace be::esi::secl::pn
{

void crack(const std::string &hashFile, const std::string &headFile, const std::string &tailsFile, const std::string &crackedFile, unsigned nb)
{

    std::ifstream hashesInput(hashFile);
    std::ifstream headsInput(headFile);
    std::ifstream tailsInput(tailsFile);
    std::ofstream crackedOutput(crackedFile);

    if (!hashesInput.is_open())
        throw std::runtime_error("Hashes file can't be opened");
    if (!headsInput.is_open())
        throw std::runtime_error("Heads file can't be opened");
    if (!tailsInput.is_open())
        throw std::runtime_error("Tails file can't be opened");
    if (!crackedOutput.is_open())
        throw std::runtime_error("Cracked file can't be opened");

    std::string hash, pwd;
    unsigned idxReduction;
    int line;
    while (std::getline(hashesInput, hash)) // For each hash
    {
        std::cout << "hash : " << hash << std::endl;
        idxReduction = NB_REDUCE - 1;
        line = findLine(hash, tailsInput, idxReduction); //Find line
        std::cout << "line : " << line << std::endl;

        if (line != -1)
        {
            pwd = findPwd(headsInput, line, idxReduction); //Find pwd in line
            std::cout << "pwd : " << pwd << std::endl;
            crackedOutput << pwd << '\n'; //Write found pwd
        }
        else
        {
            crackedOutput << '\n';
        }
    }

    hashesInput.close();
    headsInput.close();
    tailsInput.close();
    crackedOutput.close();
}

int findLine(const std::string &hash, std::ifstream &tailsInput, unsigned &idxReduction)
{
    std::string reduced = reduce(hash, idxReduction);
    int line;
    while ((line = findPositionIntoFile(reduced, tailsInput)) == -1 && (idxReduction-- > 0))
    {
        std::cout << "while line : " << line << ", idxReduction : " << idxReduction << std::endl;
        reduced = reduce(hash, idxReduction);
        for (unsigned i = idxReduction + 1; i < NB_REDUCE; i++)
        {
            std::cout << "for i : " << i << std::endl;
            reduced = reduce(getHash(reduced), i);
        }
    }
    return line;
}

int findPositionIntoFile(const std::string &str, std::ifstream &input)
{

    std::string current; //The read line
    input.clear();
    input.seekg(0, std::ios::beg);      //Set the cursor at the start of the file
    int index = 0;       //The current line index read into the file

    while (std::getline(input, current)) // For each tail
    {
        std::cout << "while getLine : " << current << std::endl;
        if (str == current)
        { //If found, return the index
            return index;
        }
        index++;
    }

    return -1; //If not found
}

std::string findPwd(std::ifstream &headsInput, const unsigned line, const unsigned idxReduction)
{
    headsInput.seekg((MAX_PWD_SIZE + 1) * line);

    std::string pwd;
    std::getline(headsInput, pwd);
    for (unsigned i = 0; i < idxReduction; i++)
    {
        pwd = reduce(getHash(pwd), i);
    }
    return pwd;
}

} //NAMESPACE be::esi::secl::pn