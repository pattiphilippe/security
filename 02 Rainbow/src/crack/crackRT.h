#ifndef CRACK_H
#define CRACK_H

#include "../generateRT/generateRT.h"
#include <string>

namespace be::esi::secl::pn
{

/**
 * Attempts to find passwords corresponding to the hashes given, at the hand of the tails.
 * With time, should replace heads and tails files by a rainbow table.
 * Crack is performed into multiple threads.
 * @param hashFile The file's name containing the hashes to crack.
 * @param headFile The file's name containing the heads.
 * @param tailsFile The file's name containing the tails.
 * @param crackedPwdFile The file's name where to print the cracked passwords.
 * @param crackedHashFile The file's name where to print the hash og the cracked passwords. Each line is the hash of the password of the same line 
 * of the passwords file.
 * @throw std::runtime_error if hashFile, headFile, tailsFile or crackedFile can't be opened.
 */
void crack(const std::string &hashFile, const std::string &headFile, const std::string &tailsFile, const std::string &crackedPwdFile, const std::string &crackedHashFile);

/**
 * Crack function to call with a thread.
 * Read and write with file is performed with mutex.
 * @param hashedInput The hashes file.
 * @param tailsInput The tails input.
 * @param crackedPwdOutput The output file where to write the passwords found.
 * @param crackedHashOutput The output file where to write the hashes which matches with the password. Each line is the hash of the password of the same line 
 * of the passwords file.
 * @param headFile The name of the head file.
 */
void crackInThread(std::ifstream &hashesInput, std::ifstream &tailsInput, std::ofstream &crackedPwdOutput, std::ofstream &crackedHashOutput, const std::string &headFile);

/**
 * Reduce the hash NB_REDUCE times and return at wich line the reduced hash is in the tailsInput file.
 * If it doesn't find the line, return -1.
 * @param hash The hash to reduce and to find.
 * @param tailsInput The file which contains the tails.
 * @param idxReduction The first reduction to do. At the end of the function, the value becomes the number of reductions to perform to get the password.
 * @return The line containing the hash, or -1 if not found.
 */
int findLine(const std::string &hash, std::ifstream &tailsInput, unsigned &idxReduction);

/**
 * Find at wich line a string is into a file.
 * Set the cursor at the start of the file to perform the search.
 * @param str The string to search.
 * @param input The input file.
 * @return the line of the string into the file. Or -1 if not found. 0 is the first line.
 */
int findPositionIntoFile(const std::string &str, std::ifstream &input);

/**
 * Perform idxReduction on the password and return the result.
 * @param pwd The password to reducd.
 * @param idxReduction The number of reduce to perform.
 * @return the password looked for
 */
std::string findPwd(std::string pwd, unsigned idxReduction);

/**
 * Get the password stored at a line.
 * @param line The line where is stored the password.
 * @param headFile The file name which contain the passwords
 * @return the password found.
 */
std::string getHeadPwdOfLine(unsigned line, const std::string &headFile);

} //NAMESPACE be::esi::secl::pn

#endif 