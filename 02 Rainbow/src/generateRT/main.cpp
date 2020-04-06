#include "generateRT.h"
#include <iostream>
#include <string>

const std::string PASSWORD_FILE("rsc/pwd.txt");
const std::string HASH_FILE("rsc/hashes.txt");
const std::string TAILS_FILE("rsc/tails.txt");

using namespace be::esi::secl::pn;

int main()
{

    //Generate NB_PASSWD passwords into pwd.txt
    //and its hashes into hashes.txt
    generatePasswords(PASSWORD_FILE, HASH_FILE);
    //Call NB_REDUCE the function to get the tail and put them into tails.txt
    generateTails(HASH_FILE, TAILS_FILE);
    //TODO: Sort tails.txt by tail
    //TODO: Remove duplicated tails to avoid collision
}