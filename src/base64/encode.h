#ifndef BASE64_ENCODE_H
#define BASE64_ENCODE_H

#include <string>

namespace b64 {

  std::string encode(const void* data, std::size_t size);

}  // b64

#endif /* BASE64_ENCODE_H */
