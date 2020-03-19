#include <iostream>
#include "vigenere.h"

using namespace std;
using namespace be::he2b::esi::sec::g43121;

int main(int argc, char * argv[])
{
    const string key = "ab";
    {

        ifstream plain("exemple\\plain.txt");
        ofstream ciphered("exemple\\ciphered.txt");

        encode(plain, ciphered, key);
    }

    {

        ifstream plain("exemple\\ciphered.txt");
        ofstream ciphered("exemple\\unciphered.txt");

        decode(plain, ciphered, key);
    }

    return 0;
}
