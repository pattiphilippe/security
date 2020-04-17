/**
 * @file crackRT.h
 * @brief Declaration of functions of crackRT.h
 */
#ifndef CRACK_H
#define CRACK_H

#include "../generateRT/generateRT.h"
#include <string>
#include <sqlite3.h>

namespace be::esi::secl::pn
{

/**
 * Select a tail
 */
inline const char *SELECT_TAIL = "SELECT tail FROM RAINBOW_TABLE WHERE tail = ?;";
/**
 * Select the head of the tail
 */
inline const char *SELECT_HEAD = "SELECT head FROM RAINBOW_TABLE WHERE tail = ?;";

/**
 * Attempts to find passwords corresponding to the hashes given, at the hand of the tails.
 * Using a opened sqlite rainbow table, should have been created with generateRT to have a good structure.
 * Crack is performed in multiple threads.
 * @param hashFile The file's name containing the hashes to crack.
 * @param db The db containing the rainbow table.
 * @param crackedPwdFile The file's name for the cracked passwords.
 * @param crackedHashFile The file's name for the hashes of the cracked passwords. Each line is the hash of the password of the same line 
 * of the passwords file.
 * @throw std::runtime_error if hashFile, headFile, tailsFile or crackedFile can't be opened.
 */
void crack(const std::string &hashFile, sqlite3 *db, const std::string &crackedPwdFile, const std::string &crackedHashFile);

/**
 * Crack function to call with a thread.
 * Read and write with file is performed with mutex.
 * @param hashedInput The hashes file.
 * @param db The DB.
 * @param crackedPwdOutput The file's name for the cracked passwords.
 * @param crackedHashOutput The file's name for the hashes of the cracked passwords. Each line is the hash of the password of the same line 
 * of the passwords file.
 */
void crackInThread(std::ifstream &hashesInput, sqlite3 *db, std::ofstream &crackedPwdOutput, std::ofstream &crackedHashOutput);

/**
 * Searches the tail of the hash and return it. Otherwise, return an empty string
 * @param hash The src hash to find.
 * @param stmtReadTail The prepared statement to search a tail in the database.
 * @param idxReduction The number of reductions performed to find the hash.
 * @return The tail of the hash, or an empty string if no tail is found.
*/
std::string getTail(const std::string &hash, sqlite3_stmt *stmtReadTail, int &idxReduction);

/**
 * Get the head of a tail.
 * @param stmtGetHead The statement to apply.
 * @param tail The tail of the search head
 * @return the head of the tail
 * @throw std::runtime_error if no tail found 
 */
std::string getHead(sqlite3_stmt *stmtGetHead, std::string tail);

/**
 * Computes the password after idxReduction reductions.
 * @param head The head of the line.
 * @param idxReduction The number of reductions to perform.
 * @return The found password.
*/
std::string findPwd(std::string head, int idxReduction);

} //NAMESPACE be::esi::secl::pn

#endif