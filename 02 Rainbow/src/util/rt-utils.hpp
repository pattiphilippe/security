/**
 * @file rt-utils.hpp
 * @brief Some utils fot the RT.
 */
#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <cmath>
#include <string>
#include <functional>

namespace be::esi::secl::pn
{

inline unsigned PWD_SIZE = 5;                                                                                                                                                                                               /**< The minimal password size */
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
inline void reduce(char digest[], int idxReduction, std::string &pwd) //TODO: pwdSize
{
    // std::size_t x = std::hash<std::string>{}(hash) + idxReduction;
    char *end = digest + 32;
    unsigned long long x = strtoull(digest, &end, 16);

    for (short i = 0; i < 8; i++)
    {
        pwd[i] = AZ_O9[x % SIZE_AZ_O9];
        x >>= 5;
    }

    //return pwd; // Implicitly treated as an rvalue
}

} //NAMESPACE be::esi::secl::pn

#endif