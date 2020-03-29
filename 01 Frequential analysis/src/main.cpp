#include "vigenere.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace be::he2b::esi::sec::g43121;

void usageError();
void printUsage();

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        usageError();
        return -1;
    }

	const string ciphersShortcut("-c");
	const string deciphersShortcut("-d");
	const string solveShortcut("-s");

    try
    {
        if (ciphersShortcut.compare(argv[1]) == 0 && argc == 5)
        {
            cout << "Ciphers :" << endl;
            //const std::string key = argv[argc - 1];
            ifstream in(argv[2]);
            ofstream out(argv[3]);
            encode(in, out, argv[argc - 1]);
            cout << "Ciphers ended" << endl;
        }
        else if (deciphersShortcut.compare(argv[1]) == 0 && argc == 5)
        {
            cout << "Deciphers in progress. Result is put in " << argv[3] << endl;
            ifstream in(argv[2]);
            ofstream out(argv[3]);
            decode(in, out, argv[4]);
            cout << "Deciphers ended" << endl;
        }
        else if (solveShortcut.compare(argv[1]) == 0 && argc == 4)
        {
            cout << "Solve :" << endl;

            ifstream inStream(argv[2]);
            if (!inStream.is_open())
                throw std::logic_error("Fichier d'entrée non ouvert");
            std::string inString;
            inStream.seekg(0, std::ios::end);
            inString.reserve(inStream.tellg());
            inStream.seekg(0, std::ios::beg);
            inString.assign((std::istreambuf_iterator<char>(inStream)),
                            std::istreambuf_iterator<char>());
            inStream.seekg(0, std::ios::beg);

            std::string key(getKey(inString));
            cout << "KEY IS : " << key << "\n";

            cout << "Deciphers in progress. Result is put in "
                 << argv[3] << endl;
            ofstream outStream(argv[3]);
            if (!outStream.is_open())
                throw std::logic_error("Erreur lors de l'ouverture de l'output");
            decode(inStream, outStream, key);
            cout << "Deciphers ended" << endl;
        }
        else
        {
            usageError();
            return -1;
        }
    }
    catch (const std::logic_error &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}

void printUsage()
{
    cout << "USAGE :" << endl;
    cout << "To ciphers : \n";
    cout << "   -c in out key" << endl;
    cout << "To deciphers : \n";
    cout << "   -d in out key" << endl;
    cout << "To solve : \n";
    cout << "   -s in out" << endl;
}

void usageError()
{
    cerr << "Mauvaise commande" << endl;
    printUsage();
}
