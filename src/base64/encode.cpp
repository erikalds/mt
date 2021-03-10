#include "encode.h"

#include "mask.h"
#include <vector>

namespace b64 {

  char lookup(std::size_t i)
  {
    static const std::vector<char> lut{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                       'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                       'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                       'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                       'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                       'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                       'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                       '4', '5', '6', '7', '8', '9', '+', '/'};
    return lut.at(i);
  }

  std::string encode(const void* data, std::size_t size)
  {
    const auto* d = static_cast<const unsigned char*>(data);
    std::string s{};
    s.reserve((size / 3) * 4);
    for (const auto* iter = d, *end = d + size; iter != end; ++iter)
    {
      unsigned char c = *iter;
      s.push_back(lookup(static_cast<std::size_t>(c >> 2U) & mask::all_six));
      if (++iter == end)
      {
        s.push_back(lookup(static_cast<std::size_t>(c << 4U) & mask::two_msb));
        s.push_back('=');
        s.push_back('=');
        break;
      }
      s.push_back(lookup((static_cast<std::size_t>(c << 4U) & mask::two_msb)
                         | (static_cast<std::size_t>(*iter >> 4U) & mask::four_lsb)));
      c = *iter;
      if (++iter == end)
      {
        s.push_back(lookup(static_cast<std::size_t>(c << 2U) & mask::four_msb));
        s.push_back('=');
        break;
      }

      s.push_back(lookup((static_cast<std::size_t>(c << 2U) & mask::four_msb)
                         | (static_cast<std::size_t>(*iter >> 6U) & mask::two_lsb)));
      s.push_back(lookup(static_cast<std::size_t>(*iter >> 0U) & mask::all_six));
    }
    return s;
  }

}  // b64
