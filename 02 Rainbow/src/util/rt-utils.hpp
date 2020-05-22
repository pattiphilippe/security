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

    inline unsigned PWD_SIZE = 5;                                                                                                                                                                                            /**< The minimal password size */
    constexpr unsigned SIZE_AZ_O9 = 36;                                                                                                                                                                                      /**< Number of valid caracters for a password */
    constexpr char AZ_O9[SIZE_AZ_O9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; /**< All valid char for a password */

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

#define SHA256_(ctx, str, in)                               \
    memset(in, 0, SHA256::DIGEST_SIZE);                     \
    ctx.init();                                             \
    ctx.update((unsigned char *)str.c_str(), str.length()); \
    ctx.final(in);

#define REDUCE(pwd, in, red_by, o_idx)                               \
    for (o_idx = 0; o_idx < PWD_SIZE; ++o_idx)                       \
    {                                                                \
        pwd[o_idx] = AZ_O9[(in[o_idx] + (red_by & 7)) % SIZE_AZ_O9]; \
        red_by >>= 3;                                                \
    }

#define SHA256_REDUCE(ctx, str, in, red_by, o_idx) \
    SHA256_(ctx, str, in)                          \
    REDUCE(str, in, red_by, o_idx)

#define SHA256_REDUCE_X_TIMES(ctx, pwd, digest, idx, red_by, cptIdx, o_idx) \
    for (; cptIdx < idx; ++cptIdx)                                          \
    {                                                                       \
        red_by = cptIdx;                                                    \
        SHA256_REDUCE(ctx, pwd, digest, red_by, o_idx)                      \
    }

} // namespace be::esi::secl::pn
#endif