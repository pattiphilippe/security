/**
 * @file generateRT.h
 * @brief Declaration of functions for generateRT.cpp
 */
#ifndef GENERATERT_H
#define GENERATERT_H

#include <string>
#include <sqlite3.h>
#include <fstream>

namespace be::esi::secl::pn
{

   constexpr unsigned NB_HEAD = 100000;                                                                                   /**< How many password we generate for the RT */
   constexpr int NB_REDUCE = 1000;                                                                                        /**< How many reduce function we use before getting the tail */
   inline const char *DROP_RT = "DROP TABLE IF EXISTS RAINBOW_TABLE;";                                                    /**< SQL statement to drop the table */
   inline const char *CREATE_RT = "CREATE TABLE RAINBOW_TABLE (head CHAR(8) PRIMARY KEY, tail CHAR(8) NOT NULL UNIQUE);"; /**< SQL statement to create the rainbow table */
   inline const char *INSERT_RT = "INSERT INTO RAINBOW_TABLE (head, tail) VALUES (?, ?);";                                /**< SQL statement to insert a head and a tail */
   inline const char *SELECT_HEAD = "SELECT head FROM RAINBOW_TABLE WHERE head = ?;";                                     /**< SQL statement to get a head from a head. This is used to check if the head exist */
   inline const std::string DB_NAME("rsc/rt.sqlite");                                                                     /**< The relative path to the DB */

   /**
    * Generate the head and the tails of the RT, and write them into the DB.
    * The tails are computed after a number of reductions, based on their hash.
    * Drop the table if exists.
    * When a merge occure, the row is not put into the DB.
    * 
    * @param db The db to store the passwords and the tails. It must be a valid db.
    * @param nbHead The number of head to generate. It should be a multiple of NB_THREADS_GENERATE.
    * @param nbReduce The number of reduction functions to apply to compute the tail. If not set, use default value.
    */
   void generateRT(sqlite3 *db, unsigned nbHead = NB_HEAD, int nbReduce = NB_REDUCE);

   /**
    * Generate the head and the tails of the RT, and write them into the DB. This function is for thread.
    * The tails are computed after a number of reductions, based on their hash.
    * 
    * @param db The db to store the passwords and the tails. It must be a valid db.
    * @param nbHead The number of head to generate.
    * @param nbReduce The number of reduction functions to apply to compute the tail. If not set, use default value.
    */
   void generateRTInThread(sqlite3 *db, unsigned nbHead, int nbReduce);

} //NAMESPACE be::esi::secl::pn

#endif