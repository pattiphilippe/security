#include <string>

#include "crackRT.h"

const std::string PASSWORD_FILE("rsc/pwd.txt");
const std::string HASH_FILE("rsc/hashes.txt");
const std::string TAILS_FILE("rsc/tails.txt");
const std::string CRACKED_FILE("rsc/cracked.txt");

using namespace be::esi::secl::pn;

int main() {

    crack(HASH_FILE, PASSWORD_FILE, TAILS_FILE, CRACKED_FILE);
}