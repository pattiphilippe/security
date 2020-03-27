#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>
#include <fstream>

namespace be::he2b::esi::sec::g43121
{

/*
 * Encode a plain text according to a key with the Vigenere algo
 */
void encode(std::string & plain, std::string & ciphered,
            const std::string & key);
/*
 * Decode a ciphered text according to the key with the Vigenere algo
 */
void decode(std::string & ciphered, std::string & plain,
            const std::string & key);
/*
 * Encode a plain text according to a key with the Vigenere algo
 */
void encode(std::ifstream & plain, std::ofstream & ciphered,
            const std::string & key);
/*
 * Decode a ciphered text according to the key with the Vigenere algo
 */
void decode(std::ifstream & ciphered, std::ofstream & plain,
            const std::string & key);

std::string getKey(std::string & in);

} //NAMESPACE be::he2b::esi::sec::g43121

#endif // VIGENERE_H
