/**
 * @file rt-utils.hpp
 * @brief Some utils fot the RT.
 */
#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <cmath>
#include <string>
#include <functional>
#include <iostream>

namespace be::esi::secl::pn
{

    inline unsigned PWD_SIZE = 5;                                                                                                                                                                                                                                                                                                                                        /**< The size of the passwords */
    constexpr unsigned SIZE_AZ_O9 = 64;                                                                                                                                                                                                                                                                                                                                  /**< Number of valid caracters for a password */
    constexpr char AZ_O9[SIZE_AZ_O9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '@', '!'}; /**< All valid char for a password */

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
 * @brief Put the hash of str into in as an decimal value.
 * 
 * @param ctx The SHA256.
 * @param str The string to hash.
 * @param in The digest : the decimal value of the hash. Its content is updated.
 */
#define SHA256_(ctx, str, in)                               \
    ctx.init();                                             \
    ctx.update((unsigned char *)str.c_str(), str.length()); \
    ctx.final(in);

/**
 * @brief Reduce a digest and put the result into a string.
 * 
 * @param pwd The reduced string. Its content is updated.
 * @param in The digest to reduce.
 * @param red_by What is the reduce value.
 * @param o_idx Cpt for the digest size.
 */
#define REDUCE(pwd, in, red_by, o_idx)                               \
    for (o_idx = 0; o_idx < PWD_SIZE; ++o_idx)                       \
    {                                                                \
        pwd[o_idx] = AZ_O9[(in[o_idx] + (red_by & 7)) % SIZE_AZ_O9]; \
        red_by >>= 3;                                                \
    }

/**
 * @brief Get the hash of an input and reduce it.
 * 
 * @param ctx The SHA256.
 * @param str The string to hash wich will contain the reduced hash.
 * @param in The digest wich contain the decimal value of the hash.
 * @param red_by What is the reduce value.
 * @param o_idx Cpt for the digest size.
 */
#define SHA256_REDUCE(ctx, str, in, red_by, o_idx) \
    SHA256_(ctx, str, in)                          \
    REDUCE(str, in, red_by, o_idx)

/**
 * @brief Get the hash of an input and reduce. Do it X times with the result (the reduced string)
 * 
 * @param ctx The SHA256.
 * @param pwd The string to hash wich will contain the reduced hash.
 * @param digest The digest wich contain the decimal value of the hash.
 * @param idx The maximal value the cpt for, the loop will take.
 * @param red_by What is the reduce value.
 * @param cptIdx The start value used for the loop. Its content is updated and set to idx.
 * @param o_idx Cpt for the digest size.
 */
#define SHA256_REDUCE_X_TIMES(ctx, pwd, digest, idx, red_by, cptIdx, o_idx) \
    for (; cptIdx < idx; ++cptIdx)                                          \
    {                                                                       \
        red_by = cptIdx;                                                    \
        SHA256_REDUCE(ctx, pwd, digest, red_by, o_idx)                      \
    }

} // namespace be::esi::secl::pn
#endif