#include <string>
#include <vector>

namespace be::esi::secl::pn
{

inline const unsigned NB_PASSWD = 100; /** How many password we generate for the RT */
inline const unsigned NB_REDUCE = 50;  /** How many reduce function we use before getting the tail */
inline const unsigned MIN_PWD_SIZE = 6; /** The minimal password size */
inline const unsigned MAX_PWD_SIZE = 8; /** The maximal password size */

/**
 * Generate a number of alphanumerical password into the file 'outputFile'
 * @param passwordFile The output file name for passwords. A file with this name will be created. If a file with this name already exist, it will be erased
 * @param hashFile The output file name for passwords hashes. A file with this name will be created. If a file with this name already exist, it will be erased
 * @param nb The number of password to generate. If not set, default number is used
 */
void generatePasswords(const std::string &passwordsFile, const std::string &hashFile, unsigned nb = NB_PASSWD);

/**
 * Reduce all hash and return the result. The hash are reduced with hash.size() reduce function.
 * @param hash A vector of all hash to reduce.
 * @return A vector of all reduced hash.
 */
std::vector<std::string> reduce(const std::vector<const std::string> &hash);

/**
 * Return the hash of a string using the SHA-256 algo.
 * @param input The string to hash.
 * @return The hash of the input.
 */
std::string hash(const std::string &input);

/**
 * Generate the tails of all hash stored into file hashFile, into the file tailsFile.
 * The tails are getted after a number of reduce, based on their hash.
 * @param hashFile The name of the file who contain the hash.
 * @param tailsFile The output file to put the tails
 * @param nb The number of reduce fonction to apply before getting the tail. If not set, use default value.
 */
void generateTails(const std::string &hashFile, const std::string &tailsFile, unsigned nb = NB_REDUCE);

} //NAMESPACE be::esi::secl::pn