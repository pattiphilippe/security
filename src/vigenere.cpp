#include "vigenere.h"

namespace be::he2b::esi::sec::g43121
{

/*
 * Encode or decode the input stream into the output stream according to the key
 */
void code(std::ifstream & in, std::ofstream & out,
          const std::string & key, bool decode = true)
{
    unsigned i = 0;
    char c = static_cast<char>(in.get());
    char k;

    while (!in.eof())
    {
        k = key[i % key.size()];
        c = decode ? c - k
            : c + k;
        out.put(c);
        c = static_cast<char>(in.get());
    }
}

void encode(std::ifstream & plain, std::ofstream & ciphered,
            const std::string & key)
{
    code(plain, ciphered, key, false);
}

void decode(std::ifstream & ciphered, std::ofstream & plain,
            const std::string & key)
{
    code(ciphered, plain, key, true);
}

} //NAMESPACE be::he2b::esi::sec::g43121
