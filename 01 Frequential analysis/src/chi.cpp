#include <cmath>
#include <string>

namespace be::he2b::esi::sec::g43121
{

inline double english[] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
    0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
    0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
    0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
    0.01974, 0.00074};

/**
 * Get the chi square value of an string.
 * The string is supposed to be in English, with upper case alphabetical values only.
 */
inline double getChiSquare(std::string &str)
{

  double sumChi = 0;
  unsigned occurence[26] = {0};

  for (char c : str)
  {
    occurence[c - 'A']++;
  }

  double expected[26];
  for (unsigned i = 0; i < 26; i++)
  {
    expected[i] = english[i] * str.length();
  }

  for (unsigned i = 0; i < 26; i++)
  {
    sumChi += std::pow(occurence[i] - expected[i], 2) / expected[i];
  }

  return sumChi;
}

} //NAMESPACE be::he2b::esi::sec::g43121
