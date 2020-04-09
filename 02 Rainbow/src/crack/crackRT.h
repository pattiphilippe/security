#ifndef CRACK_H
#define CRACK_H

#include "../generateRT/generateRT.h"
#include <string>

namespace be::esi::secl::pn
{

/**
 * Attempts to find passwords corresponding to the hashes given, at the hand of the tails.
 * With time, should replace heads and tails files by a rainbow table.
 * @param hashFile The file's name containing the hashes to crack.
 * @param headFile The file's name containing the heads.
 * @param tailsFile The file's name containing the tails.
 * @param crackedFile The file's name where to print the cracked passwords
 * @param nb The number of reduce fonction to apply before getting the tail. If not set, use default value.
 * @throw std::runtime_error if hashFile, headFile or tailsFile can't be opened.
 */
void crack(const std::string &hashFile, const std::string &headFile, const std::string &tailsFile, const std::string &crackedFile);
//TODO Make it crack different hashes on different threads to optimize cpu usage.

/**
 * Searches the line in tailsInput file that contains the hash.
 * If it doesn't find the line, line is set to -1.
 * @param hash The hash to find.
 * @param tailsInput The file which contains the tails.
 * @param idxReduction The first reduction to do. The value becomes the number of reductions to perform to get the password.
 * @return The line containing the hash, or -1 if not found.
 */
int findLine(const std::string &hash, std::ifstream &tailsInput, unsigned &idxReduction);

/**
 * Find at wich line a string is into a file.
 * Set the cursor at the start of the file.
 * @param str The string to search.
 * @param input The input file.
 * @return the line of the string into the file. Or -1 if not found. 0 is the first line.
 */
int findPositionIntoFile(const std::string &str, std::ifstream &input);

/**
 * Searches the password at line in headsInput file, doing idxReduction reductions.
 * @param headsInput The file containing the heads of the RT.
 * @param line The line containing the password.
 * @param idxReduction The number of hash and reduction to process to get the password of the line'th head
 * @return the password looked for
 */
std::string findPwd(std::string pwd, const unsigned idxReduction);

} //NAMESPACE be::esi::secl::pn


#endif 