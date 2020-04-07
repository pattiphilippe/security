#include <string>
#include <vector>
#include <unordered_map>

namespace be::esi::secl::pn
{

inline const unsigned NB_PASSWD = 100;  /** How many password we generate for the RT */
inline const unsigned NB_REDUCE = 1;    /** How many reduce function we use before getting the tail */
inline const unsigned MIN_PWD_SIZE = 8; /** The minimal password size */
inline const unsigned MAX_PWD_SIZE = 8; /** The maximal password size */
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

/**
 * Generate a number of alphanumerical password into the file 'outputFile' .
 * @param passwordFile The output file name for passwords. A file with this name will be created. If a file with this name already exist, it will be erased.
 * @param hashFile The output file name for passwords hashes. A file with this name will be created. If a file with this name already exist, it will be erased.
 * @param nb The number of password to generate. If not set, default number is used.
 * @throw std::runtime_error if passwordsFile or hashFile can't be opened.
 */
void generatePasswords(const std::string &passwordsFile, const std::string &hashFile, unsigned nb = NB_PASSWD);

/**
 * The reduce function.
 * The reduce function is very secret, so we don't tell here how it works..
 * @param hash The hash to reduce.
 * @param idxReduction The index of the hash into the table. 
 * For exemple, you hash and reduce a password 100 times. So, to reduce the first hash, 
 * idxReduction is set to 0.
 */
std::string reduce(const std::string &hash, int idxReduction); //TODO: doc

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