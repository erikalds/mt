#include "decode.h"
#include "mask.h"

#include <cctype>
#include <map>
#include <optional>

namespace b64 {

  namespace {

    inline std::optional<unsigned char> lookup(char c)
    {
      if (c == 0 || c == '=')
      {
        return std::optional<unsigned char>{};
      }

      static const std::map<char, unsigned char>
        lut{{'A',  0}, {'B',  1}, {'C',  2}, {'D',  3}, {'E',  4}, {'F',  5}, {'G',  6}, {'H',  7},
            {'I',  8}, {'J',  9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
            {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
            {'Y', 24}, {'Z', 25}, {'a', 26}, {'b', 27}, {'c', 28}, {'d', 29}, {'e', 30}, {'f', 31},
            {'g', 32}, {'h', 33}, {'i', 34}, {'j', 35}, {'k', 36}, {'l', 37}, {'m', 38}, {'n', 39},
            {'o', 40}, {'p', 41}, {'q', 42}, {'r', 43}, {'s', 44}, {'t', 45}, {'u', 46}, {'v', 47},
            {'w', 48}, {'x', 49}, {'y', 50}, {'z', 51}, {'0', 52}, {'1', 53}, {'2', 54}, {'3', 55},
            {'4', 56}, {'5', 57}, {'6', 58}, {'7', 59}, {'8', 60}, {'9', 61}, {'+', 62}, {'/', 63}};

      auto iter = lut.find(c);
      return iter->second;
    }

    template<typename T>
    unsigned char uc(T&& v)
    {
      return static_cast<unsigned char&&>(v);
    }

    char next(const char** b64data)
    {
      while (std::isspace(*++*b64data) != 0)
      {
      }
      return **b64data;
    }

  } // anonymous namespace

  void decode(const char* b64data, void* dest, std::size_t size)
  {
    auto* d = static_cast<unsigned char*>(dest);
    for (auto* iter = d, *end = d + size; iter != end; ++iter)
    {
      const auto bjp0 = lookup(*b64data);
      if (!bjp0) return;
      const auto bjp1 = lookup(next(&b64data));
      if (!bjp1) return;
      *iter = uc((*bjp0 & mask::all_six) << 2U)
        | uc((*bjp1 & mask::two_msb) >> 4U);

      const auto bjp2 = lookup(next(&b64data));
      if (++iter == end || !bjp2) return;
      *iter = uc((*bjp1 & mask::four_lsb) << 4U)
        | uc((*bjp2 & mask::four_msb) >> 2U);

      const auto bjp3 = lookup(next(&b64data));
      if (++iter == end || !bjp3) return;
      *iter = uc((*bjp2 & mask::two_lsb) << 6U)
        | uc((*bjp3 & mask::all_six) >> 0U);

      next(&b64data);
    }
  }

  std::size_t data_size(const char* b64data)
  {
    std::size_t s = 0;
    while (true)
    {
      if (*b64data == 0 || next(&b64data) == 0) break;
      ++s;
      if (next(&b64data) == 0 || *b64data == '=') break;
      ++s;
      if (next(&b64data) == 0 || *b64data == '=') break;
      ++s;
      next(&b64data);
    }
    return s;
    // std::size_t s = 0;
    // std::size_t i = 0;
    // while (true)
    // {
    //   if (b64data[i] == 0 || b64data[i + 1] == 0)
    //     break;
    //   ++s;
    //   if (b64data[i + 2] == 0 || b64data[i + 2] == '=')
    //     break;
    //   ++s;
    //   if (b64data[i + 3] == 0 || b64data[i + 3] == '=')
    //     break;
    //   ++s;
    //   i += 4;
    // }
    // return s;
  }

}  // b64
