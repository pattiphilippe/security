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

    bool getPwd(const std::string &hash, std::string &pwd, sqlite3_stmt *stmtReadTail, sqlite3_stmt *stmtReadHead,
                       unsigned char hash_dec[], unsigned char digest[], SHA256 &ctx);

#define GET_HEAD(stmtGetHead, pwd)                                               \
    sqlite3_clear_bindings(stmtGetHead);                                         \
    sqlite3_reset(stmtGetHead);                                                  \
    sqlite3_bind_text(stmtGetHead, 1, pwd.c_str(), pwd.length(), SQLITE_STATIC); \
    sqlite3_step(stmtGetHead);                                                   \
    pwd.assign(reinterpret_cast<const char *>(sqlite3_column_text(stmtGetHead, 0)));

} //NAMESPACE be::esi::secl::pn

#endif