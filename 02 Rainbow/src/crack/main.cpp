#include "crackRT.h"
#include "../util/passwd-utils.hpp"
#include <string>
#include <iostream>

const std::string PASSWORD_FILE("rsc/pwd.txt");
const std::string HASH_FILE("rsc/hashes.txt");
const std::string TAILS_FILE("rsc/tails.txt");
const std::string CRACKED_PWD_FILE("rsc/crackedPwd.txt");
const std::string CRACKED_HASH_FILE("rsc/crackedHash.txt");

using namespace be::esi::secl::pn;

int main() {

    crack(HASH_FILE, PASSWORD_FILE, TAILS_FILE, CRACKED_PWD_FILE, CRACKED_HASH_FILE);
    std::cout << "Success : " << rainbow::mass_check(CRACKED_PWD_FILE, CRACKED_HASH_FILE) << std::endl;
}