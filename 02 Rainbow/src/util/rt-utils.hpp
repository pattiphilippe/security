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
 * The reduce function.
 * The reduce function is very secret, so we don't tell here how it works..
 * @param hash The hash to reduce.
 * @param idxReduction The index of the hash into the table. To reduce the head's hash, the value is 0.
 * @param pwdMinSize The minimal password size.
 * @param pwdMaxSize The maximal password size.
 */
inline std::string reduce(const std::string &hash, int idxReduction, unsigned pwdMinSize, unsigned pwdMaxSize)
{
    unsigned long long x = std::stoull(hash.substr(0, 10), 0, 36);
    std::string pwd(pwdMaxSize, 'A');

    for (int i = 0; i < pwdMaxSize; i++)
    {
        pwd[i] = AZ_O9[x % SIZE_AZ_O9];
        x /= SIZE_AZ_O9;
    }

    return pwd;
}

/**
 * Return the hash of a string using the SHA-256 algo.
 * @param input The string to hash.
 * @return The hash of the input.
 */
inline std::string getHash(const std::string &input)
{
    return sha256(input);
}

} //NAMESPACE be::esi::secl::pn

#endif