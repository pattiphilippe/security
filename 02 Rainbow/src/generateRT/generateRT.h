/**
 * @file generateRT.h
 * @brief Declaration of functions for generateRT.cpp
 */
#ifndef GENERATERT_H
#define GENERATERT_H

#include <string>
#include <sqlite3.h>

namespace be::esi::secl::pn
{

inline unsigned NB_HEAD = 255000000;                                                                                   /**< The default number of heads(/rows) to generate for the RT */
inline int NB_REDUCE = 10000;                                                                                          /**< The default number of reduce function to apply before getting the tail */
inline const unsigned PWD_SIZE = 6;                                                                                    /**< The default password's size */
inline const char *DROP_RT = "DROP TABLE IF EXISTS RAINBOW_TABLE;";                                                    /**< Drop the table if exist*/
inline const char *CREATE_RT = "CREATE TABLE RAINBOW_TABLE (head CHAR(8) PRIMARY KEY, tail CHAR(8) NOT NULL UNIQUE);"; /**< Create the table */
inline const char *INSERT_RT = "INSERT INTO RAINBOW_TABLE (head, tail) VALUES (?, ?);";                                /**< Insert a row into the table*/
inline const std::string DB_NAME("rsc/rt.sqlite");                                                                     /**< The relative path to the DB */

/**
 * Generate the head and the tails of the RT, and write them into the DB.
 * The tails are computed after a number of reductions, based on their hash.
 * Drop the table if exists.
 * When a merge occure, the row is not put into the DB.
 * @param db The db to store the passwords and the tails. It must be a valid db.
 * @param nbHead The number of head to generate.
 * It is recommended to use a multiple of NB_THREAD. Otherwise, the number of heads can be a bit different as expected.
 * @param nbReduce The number of reduction functions to apply to compute the tail. If not set, use default value.
 * @param pwdSize The passwords's lenght of the table
 */
void generateRT(sqlite3 *db, unsigned nbHead = NB_HEAD, int nbReduce = NB_REDUCE, unsigned pwdSize = PWD_SIZE);

/**
 * Generate the head and the tails of the RT, and write them into the DB. This function is for thread.
 * The tails are computed after a number of reductions, based on their hash.
 * @param db The db to store the passwords and the tails. It must be a valid db.
 * @param nbHead The number of head to generate.
 * @param nbReduce The number of reduction functions to apply to compute the tail. If not set, use default value.
 * @param pwdSize The passwords's lenght of the table
 */
void generateRTInThread(sqlite3 *db, unsigned nbHead, int nbReduce, unsigned pwdSize);

} //NAMESPACE be::esi::secl::pn

#endif