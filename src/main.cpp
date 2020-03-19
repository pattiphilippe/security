#include <iostream>
#include "vigenere.h"
#include <cstring>

using namespace std;
using namespace be::he2b::esi::sec::g43121;

void usageError();
void printUsage();

int main(int argc, char *argv[])
{

    if (argc < 5)
    {
        usageError();
        return -1;
    }

    const string key = argv[argc - 1];
    ifstream in(argv[2]);
    ofstream out(argv[3]);

    try
    {
        if (strcmp(argv[1], "-e"))
        {
            encode(in, out, key);
        }
        else if (strcmp(argv[1], "-d"))
        {
            decode(in, out, key);
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
    cout << "To encode : \n";
    cout << "   -e in out key" << endl;
    cout << "To decode : \n";
    cout << "   -d in out key" << endl;
}

void usageError()
{
    cerr << "Mauvaise commande" << endl;
    printUsage();
}