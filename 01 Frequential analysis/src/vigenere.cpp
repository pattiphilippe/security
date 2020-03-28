#include "vigenere.h"
#include "IC.hpp"
#include "chi.hpp"
#include <stdexcept>
#include <array>
#include <vector>

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
void code(std::string &in, std::string &out,
          const std::string &key, bool decode = true)
{
    unsigned i = 0;
    char c;
    std::string k = getNormalizedKey(key);
    out.reserve(in.length());

    for (char c : in)
    {
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        else if (c < 'A' || c > 'Z')
            continue;
        c = decode ? (c - k[i] + 26) % 26 + 'A'
                   : (c + k[i] - 2 * 'A') % 26 + 'A';
        out += c;
        i = (i + 1) % k.length();
    }
}
void code(std::ifstream &in, std::ofstream &out,
          const std::string &key, bool decode = true)
{
    if (!in.is_open())
        throw std::logic_error("Erreur lors de l'ouverture de l'input");
    if (!out.is_open())
        throw std::logic_error("Erreur lors de l'ouverture de l'output");

    unsigned i = 0;
    int c = in.get();
    std::string k = getNormalizedKey(key);

    while (!in.eof())
    {
        if (c >= 'a' && c <= 'z')
            c += 'A' - 'a';
        if (c >= 'A' && c <= 'Z')
        {
            c = decode ? (c - k[i] + 26) % 26 + 'A'
                       : (c + k[i] - 2 * 'A') % 26 + 'A';
            out.put(c);
            i = (i + 1) % k.length();
        }
        c = in.get();
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

void encode(std::string &plain, std::string &ciphered,
            const std::string &key)
{
    code(plain, ciphered, key, false);
}

void decode(std::string &ciphered, std::string &plain,
            const std::string &key)
{
    code(ciphered, plain, key, true);
}

unsigned getKeyLenght(std::string &big, std::array<std::vector<std::string>, 20> &parts)
{
    double icAverage[26];

    for (unsigned i = 1; i < 20; i++)
    {
        int posPart = 0;

        for (const char &c : big)
        {
            parts[i].at(posPart) += c;
            posPart = (posPart + 1) % i;
        }

        double sum = 0;
        for (auto &s : parts[i])
        {
            sum += calculateIC(s);
        }
        icAverage[i] = sum / i;
    }

    double maxAvg = icAverage[1];
    unsigned maxIndex = 1;
    for (unsigned i = 2; i < 20; i++)
    {
        if (icAverage[i] > maxAvg)
        {
            maxAvg = icAverage[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

std::string getKeyCesar(std::string &in)
{
    std::string ciph;
    double chi[26] = {0};

    for (char k = 'A'; k <= 'Z'; k++)
    {
        ciph.clear();
        decode(in, ciph, std::string(1, k));
        chi[k - 'A'] = getChiSquare(ciph);
    }

    double minimum = chi[0];
    std::string soluce(1, 'A');

    for (unsigned c = 1; c < 26; c++)
    {
        if (chi[c] < minimum)
        {
            minimum = chi[c];
            soluce = c + 'A';
        }
    }

    return soluce;
}

std::string getKey(std::string &in)
{
    std::string key;
    std::array<std::vector<std::string>, 20> parts;
    for (unsigned i = 1; i < 20; i++)
    {
        parts.at(i).reserve(i);
        for (unsigned pI = 0; pI < i; pI++)
        {
            parts.at(i).push_back(std::string());
        }
    }
    unsigned keyLength = getKeyLenght(in, parts);

    for (auto &part : parts[keyLength])
    {
        key += getKeyCesar(part);
    }

    return key;
}

} //NAMESPACE be::he2b::esi::sec::g43121
