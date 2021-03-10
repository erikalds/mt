#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

#include <cstddef>

namespace b64 {

  void decode(const char* b64data, void* dest, std::size_t size);

  std::size_t data_size(const char* b64data);

}  // b64

#endif /* BASE64_DECODE_H */
