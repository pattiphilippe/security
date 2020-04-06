#include "generateRT.h"
#include <iostream>
#include <string>

const std::string PASSWORD_FILE("rsc/pwd.txt");
const std::string HASH_FILE("rsc/hashes.txt");
const std::string TAILS_FILE("rsc/tails.txt");

using namespace be::esi::secl::pn;

int main()
{

    //Generate NB_PASSWD passwords into pwd.txt
    //and its hashes into hashes.txt
    //generatePasswords(PASSWORD_FILE, HASH_FILE);
    //Call NB_REDUCE the function to get the tail and put them into tails.txt
    //generateTails(HASH_FILE, TAILS_FILE);
    //TODO: Sort tails.txt by tail
    //TODO: Remove duplicated tails to avoid collision
    std::string hash("Some fucking hashSome fucking hashSome fucking hash");
    int pwdInt[MAX_PWD_SIZE];
    for (unsigned i = 0; i < MAX_PWD_SIZE; ++i)
    {
        pwdInt[MAX_PWD_SIZE - i - 1] = hash[hash.length() - i - 1];
    }

    pwdInt[MAX_PWD_SIZE-1] += 3; //+= the index. 3 as exemple
    //TODO: what if not alphanumeric ?

    std::string pwd;
    for (unsigned i = 0; i< MAX_PWD_SIZE;++i) {
        pwd += static_cast<char>(pwdInt[i]);
    }

    std::cout << pwd << std::endl;
    //tab = [0, 1]
    // pwd [0] = tab[x%2] => 0 ou 1
    //
    // 4 : 00 01 10 11
    // 
    // pwd [0] = tab[x%2]1
    // pwd [1] = tab[(x%(2^2) /2)] => %4 /2 7%4 = 3 -> 3/2 = 1

    // %(nbPossibilités ^ nbChar)






    /*
    A => 65
    AA => 6565 OU 65+65=130
    [65-80]*256 = 100*256 = 25 600 % 36^8

    6565+1 => AB
    hash = "mlkdjmlqkfjijjé34j3jdic764ddsjd" => pwd = 764ddsjd + 50_000 

    
    764ddsjd +1 = 764ddsje
    764ddsjd +37 = 764ddsjd +1 +36 = 764ddsjd +1 +36*1  = 764ddske 
    764ddsjd +37 = 764ddsjd + (37%36) + (37/36) = 764ddsjd +1 + 1 = 764ddske 
    764ddsjd +38 = 764ddsjd +2 +36 = 764ddsjd +1 +36*1  = 764ddskf 

    pwd = "764ddsjd"
    add = 38
    for(int i = 7; i >= 0; i--) {
        pwd[i] = tab[ ( index(pwd[i]) + (add%36) ) % 36 ]
        add /= 36
    }

    pwd 10024553201132
    %10
    +2, +3, +1, +1, +0, 0+,  

    unsigned long long int (8 bytes ou 64 bits) 
    50000 en binaire = 1100 0011 0101 0000

    tab = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    pwd [0] = tab[x%10]

    hash + X = hash + X 
    hash = hash 

    // _ _
    // +12
    // x = 200
    // (200 % +1)% (10^2)






    */
}