#include "vigenere.h"
#include <stdexcept>
#include <iostream>

namespace be::he2b::esi::sec::g43121
{

std::string getNormalizedKey(const std::string &key)
{
    std::string result;
    result.reserve(key.length());

    for (int i = 0; i < key.size(); ++i)
    {
        if (key[i] >= 'A' && key[i] <= 'Z')
            result += key[i];
        else if (key[i] >= 'a' && key[i] <= 'z')
            result += key[i] + 'A' - 'a';
    }

    return result;
}

/*
 * Encode or decode the input stream into the output stream according to the key
 */
void code(std::ifstream &in, std::ofstream &out,
          const std::string &key, bool decode = true)
{
    if (!in.is_open())
    {
        throw std::logic_error("Fichier d'entrée non ouvert");
    }
    if (!out.is_open())
    {
        throw std::logic_error("Fichier de sortie non ouvert");
    }

    unsigned i = 0;
    char c;
    std::string k = getNormalizedKey(key);

    while (!in.eof())
    {
        c = static_cast<char>(in.get());
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        else if (c < 'A' || c > 'Z')
            continue;
        c = decode ? (c - k[i] + 26) % 26 + 'A'
                   : (c + k[i] - 2 * 'A') % 26 + 'A';
        out.put(c);
        i = (i + 1) % k.length();
    }
}

void encode(std::ifstream &plain, std::ofstream &ciphered,
            const std::string &key)
{
    code(plain, ciphered, key, false);
}

void decode(std::ifstream &ciphered, std::ofstream &plain,
            const std::string &key)
{
    code(ciphered, plain, key, true);
}

} //NAMESPACE be::he2b::esi::sec::g43121
