/**
 * @file generateRT.h
 * @brief Declaration of functions for generateRT.cpp
 */
#ifndef GENERATERT_H
#define GENERATERT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sqlite3.h>

namespace be::esi::secl::pn
{

inline const unsigned NB_HEAD = 500000000;                                                                                                                                                                                                                                                                                                                    /**< How many password we generate for the RT */
inline const int NB_REDUCE = 160;                                                                                                                                                                                                                                                                                                                             /**< How many reduce function we use before getting the tail */
inline const unsigned MIN_PWD_SIZE = 6;                                                                                                                                                                                                                                                                                                                       /**< The minimal password size */
inline const unsigned MAX_PWD_SIZE = 6;                                                                                                                                                                                                                                                                                                                       /**< The maximal password size */
inline const unsigned SIZE_AZ_O9 = 62;                                                                                                                                                                                                                                                                                                                        /**< Number of valid caracters for a password */
inline const char AZ_O9[SIZE_AZ_O9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; /**< All valid char for a password */
inline const char *DROP_RT = "DROP TABLE IF EXISTS RAINBOW_TABLE;";
inline const char *CREATE_RT = "CREATE TABLE RAINBOW_TABLE (head CHAR(8) PRIMARY KEY, tail CHAR(8) NOT NULL UNIQUE);";
inline const char *INSERT_RT = "INSERT OR IGNORE INTO RAINBOW_TABLE (head, tail) VALUES (?, ?);";
inline const std::string DB_NAME("rsc/rt_6_1_500000000_160.sqlite"); /**< The relative path to the DB */

/**
 * The reduce function.
 * The reduce function is very secret, so we don't tell here how it works..
 * @param hash The hash to reduce.
 * @param idxReduction The index of the hash into the table. 
 * For exemple, you hash and reduce a password 100 times. So, to reduce the first hash, 
 * idxReduction is set to 0.
 */
std::string reduce(const std::string &hash, int idxReduction);

/**
 * Return the hash of a string using the SHA-256 algo.
 * @param input The string to hash.
 * @return The hash of the input.
 */
std::string getHash(const std::string &input);

/**
 * Generate the head and the tails of the RT, and write them into the DB.
 * The tails are computed after a number of reductions, based on their hash.
 * Drop the table if exists.
 * @param db The db to store the passwords and the tails. It must be a valid db.
 * @param nbHead The number of head to generate.
 * @param nbReduce The number of reduction functions to apply to compute the tail. If not set, use default value.
 */
void generateRT(sqlite3 *db, unsigned nbHead = NB_HEAD, int nbReduce = NB_REDUCE);

/**
 * Generate the head and the tails of the RT, and write them into the DB. This function is for thread.
 * The tails are computed after a number of reductions, based on their hash.
 * @param db The db to store the passwords and the tails. It must be a valid db.
 * @param nbHead The number of head to generate.
 * @param nbReduce The number of reduction functions to apply to compute the tail. If not set, use default value.
 */
void generateRTInThread(sqlite3 *db, unsigned nbHead, int nbReduce);

} //NAMESPACE be::esi::secl::pn

#endif