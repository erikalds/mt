#ifndef MASK_H
#define MASK_H

namespace b64::mask {

  constexpr static const unsigned int all_six{0x3F};
  constexpr static const unsigned int two_lsb{0x03};
  constexpr static const unsigned int two_msb{0x30};
  constexpr static const unsigned int four_lsb{0x0F};
  constexpr static const unsigned int four_msb{0x3C};

}  // b64::mask

#endif /* MASK_H */
