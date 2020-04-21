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

inline unsigned PWD_SIZE = 8;                                                                                                                                                                                               /**< The minimal password size */
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

inline void reduce(std::string &pwd, const uint8_t *in, int red_by)
{
    static unsigned o_idx;
    red_by %= 32;
    for (o_idx = 0; o_idx < PWD_SIZE; o_idx++)
    {
        pwd[o_idx] = AZ_O9[in[red_by] % SIZE_AZ_O9];
        red_by = (red_by + 9) % 32;
    }
}

inline void mystrtoull(unsigned char digest[], unsigned long long &x)
{
    x = 0;
    char c;
    for (int i = 0, mult = 1; i < 11; i++)
    {
        printf("digest[i] as u : %u ", digest[i]);
        c = digest[i];
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= ('a' - 10);
        printf(", c : %d ", c);
        x += c * mult;
        mult *= 16;
    }
}

/**
 * The reduce function for passwords of lenght 6, 7 or 8.
 * The reduce function is very secret, so we don't tell here how it works..
 * It generate more passwords of lenght 8 than lenght 7, and more lenght 7 than lenght 6, predetermined by a chosen success rate (37%, 98%, 97%).
 * @param hash The hash to reduce.
 * @param idxReduction The index of the hash into the table. To reduce the head's hash, the value is 0.
 */
inline void OLDreduce(unsigned char digest[], int idxReduction, std::string &pwd) //TODO: pwdSize
{
    // std::size_t x = std::hash<std::string>{}(hash) + idxReduction;
    //char *end = &(digest[32]);
    printf("in reduce digest before strtoull : \n");
    for (int i = 0; i < 32; i++)
        printf("%02x", (unsigned char)digest[i]);
    printf("\n");
    //const char * test = "2e5e08fed97 3a586ecae2e6c2fbf4e6c88f0eac3c8a61aaf46214c912c85e2b4";
    // unsigned char * test_u = (unsigned char *)test;
    // digest[12] = ' ';
    unsigned long long x; // = strtoull((char*)test, NULL, 16);
    mystrtoull(digest, x);
    printf("reduce , x : %llu\n", x);

    for (int i = 0; i < 8; i++)
    {
        pwd[i] = AZ_O9[x % SIZE_AZ_O9];
        x >>= 5;
    }

    //return pwd; // Implicitly treated as an rvalue
}

} //NAMESPACE be::esi::secl::pn

#endif