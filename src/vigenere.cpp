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

void encode(std::ifstream &plain, std::ofstream &ciphered,
            const std::string &key)
{
    if (!plain.is_open())
    {
        throw std::logic_error("Fichier d'entrée non ouvert");
    }
    if (!ciphered.is_open())
    {
        throw std::logic_error("Fichier de sortie non ouvert");
    }
    unsigned i = 0;
    char c;
    std::string k = getNormalizedKey(key);

    while (!plain.eof())
    {
        c = static_cast<char>(plain.get());
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        else if (c < 'A' || c > 'Z')
            continue;
        c = (c + k[i] - 2 * 'A') % 26 + 'A';
        ciphered.put(c);
        i = (i + 1) % k.length();
    }
}

void decode(std::ifstream &ciphered, std::ofstream &plain,
            const std::string &key)
{
    if (!ciphered.is_open())
    {
        throw std::logic_error("Fichier d'entrée non ouvert");
    }
    if (!plain.is_open())
    {
        throw std::logic_error("Fichier de sortie non ouvert");
    }
    unsigned i = 0;
    char c;
    std::string k = getNormalizedKey(key);

    while (!ciphered.eof())
    {
        c = static_cast<char>(ciphered.get());
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        else if (c < 'A' || c > 'Z')
            continue;
        c = (c - k[i] + 26) % 26 + 'A';
        plain.put(c);
        i = (i + 1) % k.length();
    }
}

} //NAMESPACE be::he2b::esi::sec::g43121
