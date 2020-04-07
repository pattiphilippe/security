#include "generateRT.h"
#include <iostream>
#include <string>
#include <unordered_map>

const std::string PASSWORD_FILE("rsc/pwd.txt");
const std::string HASH_FILE("rsc/hashes.txt");
const std::string TAILS_FILE("rsc/tails.txt");

using namespace be::esi::secl::pn;

int main()
{

    //Generate NB_PASSWD passwords into pwd.txt
    //and its hashes into hashes.txt
    generatePasswords(PASSWORD_FILE, HASH_FILE);
    //Call NB_REDUCE the function to get the tail and put them into tails.txt
    generateTails(HASH_FILE, TAILS_FILE);
    //TODO: Sort tails.txt by tail
    //TODO: Remove duplicated tails to avoid collision
    /*
    std::unordered_map<char, int> u = {
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
    for (const auto &n : u)
    {
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    }

    std::cout << "u['A'] : " << u['A'] << std::endl;
    std::cout << "u['a'] : " << u['a'] << std::endl;
    std::cout << "u['0'] : " << u['0'] << std::endl;
    char c = 'a';
    std::cout << "char c = 'a' , u[c] : " << u[c] << std::endl;
    */
}