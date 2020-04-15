#ifndef GENERATERT_H
#define GENERATERT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sqlite3.h>

namespace be::esi::secl::pn
{

inline const unsigned NB_PASSWD = 2000;  /** How many password we generate for the RT */
inline const unsigned NB_REDUCE = 10000; /** How many reduce function we use before getting the tail */
inline const unsigned MIN_PWD_SIZE = 6;  /** The minimal password size */
inline const unsigned MAX_PWD_SIZE = 6;  /** The maximal password size */
inline const unsigned SIZE_AZ_O9 = 62;
inline const char AZ_O9[SIZE_AZ_O9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
inline std::unordered_map<char, int> ID_AZ_O9 = {
    {'A', 0},
    {'B', 1},
    {'C', 2},
    {'D', 3},
    {'E', 4},
    {'F', 5},
    {'G', 6},
    {'H', 7},
    {'I', 8},
    {'J', 9},
    {'K', 10},
    {'L', 11},
    {'M', 12},
    {'N', 13},
    {'O', 14},
    {'P', 15},
    {'Q', 16},
    {'R', 17},
    {'S', 18},
    {'T', 19},
    {'U', 20},
    {'V', 21},
    {'W', 22},
    {'X', 23},
    {'Y', 24},
    {'Z', 25},
    {'a', 26},
    {'b', 27},
    {'c', 28},
    {'d', 29},
    {'e', 30},
    {'f', 31},
    {'g', 32},
    {'h', 33},
    {'i', 34},
    {'j', 35},
    {'k', 36},
    {'l', 37},
    {'m', 38},
    {'n', 39},
    {'o', 40},
    {'p', 41},
    {'q', 42},
    {'r', 43},
    {'s', 44},
    {'t', 45},
    {'u', 46},
    {'v', 47},
    {'w', 48},
    {'x', 49},
    {'y', 50},
    {'z', 51},
    {'0', 52},
    {'1', 53},
    {'2', 54},
    {'3', 55},
    {'4', 56},
    {'5', 57},
    {'6', 58},
    {'7', 59},
    {'8', 60},
    {'9', 61},
};
inline const char *DROP_RT = "DROP TABLE RAINBOW_TABLE;";
inline const char *CREATE_RT = "CREATE TABLE RAINBOW_TABLE (head CHAR(8) PRIMARY KEY, tail CHAR(8) NOT NULL UNIQUE);";
inline const char *INSERT_RT = "INSERT INTO RAINBOW_TABLE (head, tail) VALUES (?, ?);";

/**
 * The reduce function.
 * The reduce function is very secret, so we don't tell here how it works..
 * @param hash The hash to reduce.
 * @param idxReduction The index of the hash into the table. 
 * For exemple, you hash and reduce a password 100 times. So, to reduce the first hash, 
 * idxReduction is set to 0.
 */
std::string reduce(const std::string &hash, unsigned idxReduction);

/**
 * Return the hash of a string using the SHA-256 algo.
 * @param input The string to hash.
 * @return The hash of the input.
 */
std::string getHash(const std::string &input);

/**
 * Generate the head and the tails of the RT, and write them into the DB.
 * The tails are computed after a number of reductions, based on their hash.
 * @param db The db to store the passwords and the tails. It must be a valid db.
 * @param nb The number of reduction functions to apply to compute the tail. If not set, use default value.
 */
void generateRT(sqlite3 *db, unsigned nbReduce = NB_REDUCE);

} //NAMESPACE be::esi::secl::pn

#endif