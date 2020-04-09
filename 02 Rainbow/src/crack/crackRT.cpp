#include "crackRT.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>

namespace be::esi::secl::pn
{

std::mutex mtxReadHead;
std::mutex mtxPrintCracked;
const unsigned NB_THREADS = 10;

/** Set the number of caracters to get a new line in a file */
#ifdef _WIN32
const unsigned NB_ESCAPE_CHAR = 2;
#elif
const unsigned NB_ESCAPE_CHAR = 1;
#endif

std::string getHeadPwdOfLine(unsigned line, const std::string &headFile)
{
    std::ifstream headsInput(headFile);
    if (!headsInput.is_open())
        throw std::runtime_error("Heads file can't be opened");
    std::string pwd;

    headsInput.seekg((MAX_PWD_SIZE + NB_ESCAPE_CHAR) * line);
    std::getline(headsInput, pwd);

    headsInput.close();

    return pwd;
}

void crackInThread(std::ifstream &hashesInput, std::ifstream &tailsInput, std::ofstream &crackedOutput, const std::string &headFile)
{
    std::string hash, pwd;
    unsigned idxReduction;
    int line;

    mtxReadHead.lock();
    std::getline(hashesInput, hash);
    mtxReadHead.unlock();
    while (hashesInput) // For each hash
    {
        idxReduction = NB_REDUCE - 1;
        line = findLine(hash, tailsInput, idxReduction); //Find line

        if (line != -1)
        {
            pwd = findPwd(getHeadPwdOfLine(line, headFile), idxReduction); //Find pwd of the password of line x
            mtxPrintCracked.lock();
            crackedOutput << hash << ';' << pwd << '\n'; //Write found pwd
            //TODO: write hash in another file
            mtxPrintCracked.unlock();
        }
        mtxReadHead.lock();
        std::getline(hashesInput, hash);
        mtxReadHead.unlock();
    }
}

void crack(const std::string &hashFile, const std::string &headFile, const std::string &tailsFile, const std::string &crackedFile)
{

    std::ifstream hashesInput(hashFile);
    std::ifstream tailsInput(tailsFile);
    std::ofstream crackedOutput(crackedFile);

    if (!hashesInput.is_open())
        throw std::runtime_error("Hashes file can't be opened");
    if (!tailsInput.is_open())
        throw std::runtime_error("Tails file can't be opened");
    if (!crackedOutput.is_open())
        throw std::runtime_error("Cracked file can't be opened");

    std::vector<std::thread> threads;
    for (unsigned i = 0; i < NB_THREADS; i++)
    {
        threads.push_back(std::thread(crackInThread, std::ref(hashesInput), std::ref(tailsInput), std::ref(crackedOutput), std::ref(headFile)));
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });

    hashesInput.close();
    tailsInput.close();
    crackedOutput.close();
}

int findLine(const std::string &hash, std::ifstream &tailsInput, unsigned &idxReduction)
{
    std::string reduced = reduce(hash, idxReduction);
    int line;
    while ((line = findPositionIntoFile(reduced, tailsInput)) == -1 && (idxReduction-- > 0))
    {
        reduced = reduce(hash, idxReduction);
        for (unsigned i = idxReduction + 1; i < NB_REDUCE; i++)
        {
            reduced = reduce(getHash(reduced), i);
        }
    }
    return line;
}

int findPositionIntoFile(const std::string &str, std::ifstream &input)
{

    std::string current; //The read line
    input.clear();
    input.seekg(0); //Set the cursor at the start of the file
    int index = 0;  //The current line index read into the file

    while (std::getline(input, current)) // For each tail
    {
        if (str == current)
        { //If found, return the index
            return index;
        }
        index++;
    }

    return -1; //If not found
}

std::string findPwd(std::string pwd, const unsigned idxReduction)
{
    for (unsigned i = 0; i < idxReduction; i++)
    {
        pwd = reduce(getHash(pwd), i);
    }
    return pwd;
}

} //NAMESPACE be::esi::secl::pn