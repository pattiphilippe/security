/**
 * @file crackRT.h
 * @brief Declaration of functions of crackRT.h
 */
#ifndef CRACK_H
#define CRACK_H

#include "../util/sha256.h"
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
    inline const char *SELECT_HEAD_T = "SELECT head FROM RAINBOW_TABLE WHERE tail = ?;";

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
     * @brief Get the password of a given hash.
     * 
     * @param hash The hash to crack.
     * @param pwd The password found. This var is modified after the call of the function. But its content have no sense 
     * if the function return false.
     * @param stmtReadTail Statement to get a tail from the DB according to a tail. This mean that this is used to know 
     * if the tail exist in the DB.
     * @param stmtReadHead Statement to get the head of a tail.
     * @param hash_dec The decimal hash to crack.
     * @param digest Var to hold temporary digest, to avoid multiple creation. It is used to compare with the hash to crack.
     * @param ctx The SHA256 class.
     * @return true If there is a collision. 'pwd' don't hold a valid value.
     * @return false If there is no collition. The password of the hash is hols in 'pwd'.
     */
    bool getPwd(const std::string &hash, std::string &pwd, sqlite3_stmt *stmtReadTail, sqlite3_stmt *stmtReadHead,
                unsigned char hash_dec[], unsigned char digest[], SHA256 &ctx);

/**
 * @brief Get the head of a tail.
 * 
 * @param stmtGetHead The statement to get the head with a tail. The tail must exist in the DB.
 * @param pwd The password where to save the head.
 */
#define GET_HEAD(stmtGetHead, pwd)                                               \
    sqlite3_clear_bindings(stmtGetHead);                                         \
    sqlite3_reset(stmtGetHead);                                                  \
    sqlite3_bind_text(stmtGetHead, 1, pwd.c_str(), pwd.length(), SQLITE_STATIC); \
    sqlite3_step(stmtGetHead);                                                   \
    pwd.assign(reinterpret_cast<const char *>(sqlite3_column_text(stmtGetHead, 0)));

} //NAMESPACE be::esi::secl::pn

#endif