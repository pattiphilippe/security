#include <fstream>
#include <string>

namespace be::he2b::esi::sec::g43121
{

/**
 * Get the IC of a string.
 * All non upper case alphabetical values is ignored.
 */
inline double calculateIC(const std::string &s)
{
    int N = 0;
    double sum = 0.0;
    double total = 0.0;

    int occurences[26] = {0};

    //calculate frequency of each letter
    int c;
    for (int i = 0; i < s.length(); i++)
    {
        c = s[i] - 'A';
        if (c >= 0 && c < 26)
        {
            occurences[c]++;
            N++;
        }
    }

    //calculate the sum of each frequency
    for (int i = 0; i < 26; i++)
    {
        c = occurences[i];
        sum = sum + (c * (c - 1));
    }

    total = sum / N / (N - 1);

    return total;
}

} //NAMESPACE be::he2b::esi::sec::g43121
