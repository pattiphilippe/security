/**
 * @file rt-utils.hpp
 * @brief Some utils fot the RT.
 */
#ifndef RT_UTILS_H
#define RT_UTILS_H

#include "../util/sha256.h"
#include <cmath>
#include <string>

namespace be::esi::secl::pn
{

inline const unsigned SIZE_AZ_O9 = 36;                                                                                                                                                                                      /**< Number of valid caracters for a password */
inline const char AZ_O9[SIZE_AZ_O9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; /**< All valid char for a password */

/**
 * Get the percentage of success for a RT.
 * 
 * @param mn How many rows in the table.
 * @param nbReduce How many reduce to apply before getting the tail.
 * @param nbChar Lenght of the password.
 * @param nbPossibleValues Number of value that can take a char of the password
 * @return the percentage of success for a RT. 
 */
inline double getPercentage(double mn, int nbReduce, int nbChar, unsigned nbPossibleValues)
{
    unsigned long long N = std::pow(nbPossibleValues, nbChar);
    double temp = 1;

    for (int i = 1; i <= nbReduce; i++)
    {
        mn = N * (1 - std::exp(-mn / N));
        temp *= 1 - mn / N;
    }
    return 1 - temp;
}

/**
 * The reduce function for passwords of lenght 6, 7 or 8.
 * The reduce function is very secret, so we don't tell here how it works..
 * It generate more passwords of lenght 8 than lenght 7, and more lenght 7 than lenght 6, predetermined by a chosen success rate (37%, 98%, 97%).
 * @param hash The hash to reduce.
 * @param idxReduction The index of the hash into the table. To reduce the head's hash, the value is 0.
 */
inline std::string reduce(const std::string &hash, int idxReduction)
{
    //unsigned long long x = (std::hash(hash.substr(0, 8), 0, 36) << 8) + std::hash(hash.substr(8, 8), 0, 36) + idxReduction; //TODO Check if better
    unsigned long long x = (std::stoull(hash.substr(0, 8), 0, 36) << 8) + std::stoull(hash.substr(8, 8), 0, 36) + idxReduction;
    //int pwdSize = std::hash(hash.substr(0, 2), 0, 36); //TODO Check
    int pwdSize = std::stoi(hash.substr(0, 2), 0, 36);
    if (pwdSize < 27)
        pwdSize = 6;
    else if (pwdSize < 535)
        pwdSize = 7;
    else
        pwdSize = 8;

    std::string pwd(pwdSize, 'A');
    while (pwdSize--)
    {
        pwd[pwdSize] = AZ_O9[x % SIZE_AZ_O9];
        x >>= 5;
    }

    return pwd; //TODO return with && (move sem)
}

/**
 * Return the hash of a string using the SHA-256 algo.
 * @param input The string to hash.
 * @return The hash of the input.
 */
inline std::string getHash(const std::string &input)
{
    return sha256(input);
    //TODO move sem
}

} //NAMESPACE be::esi::secl::pn

#endif