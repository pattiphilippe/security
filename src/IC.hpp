#include <fstream>
#include <string>

inline double calculateIC(const std::string & s)
{
    int i;
    int N = 0;
    double sum = 0.0;
    double total = 0.0;

    //initialize array of values to count frequency of each letter
    int values[26];
    for (i = 0; i < 26; i++)
    {
        values[i] = 0;
    }

    //calculate frequency of each letter in s
    int ch;
    for (i = 0; i < s.length(); i++)
    {
        ch = s[i] - 'A';
        if (ch >= 0 && ch < 26)
        {
            values[ch]++;
            N++;
        }
    }

    //calculate the sum of each frequency
    for (i = 0; i < 26; i++)
    {
        ch = values[i];
        sum = sum + (ch * (ch - 1));
    }

    //divide by N(N-1)
    total = sum / N / (N - 1);

    //return the result
    return total;
}