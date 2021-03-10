#include "base64/encode.h"

#include <catch2/catch.hpp>

TEST_CASE("encode all", "[base64]")
{
  std::vector<char> v{'\x00', '\x10', '\x83'};
  auto s = b64::encode(&v[0], v.size() * sizeof(v[0]));
  CHECK("ABCD" == s);

  v = std::vector<char>{'\x00', '\x10', '\x83',
                        '\x10', '\x51', '\x87'};
  s = b64::encode(&v[0], v.size() * sizeof(v[0]));
  CHECK("ABCDEFGH" == s);

  v = std::vector<char>{'\x00', '\x10', '\x83',  // ABCD
                        '\x10', '\x51', '\x87',  // EFGH
                        '\x20', '\x92', '\x8B',  // IJKL
                        '\x30', '\xD3', '\x8F',  // MNOP
                        '\x41', '\x14', '\x93',  // QRST
                        '\x51', '\x55', '\x97',  // UVWX
                        '\x61', '\x96', '\x9B',  // YZab
                        '\x71', '\xD7', '\x9F',  // cdef
                        '\x82', '\x18', '\xA3',  // ghij
                        '\x92', '\x59', '\xA7',  // klmn
                        '\xA2', '\x9A', '\xAB',  // opqr
                        '\xB2', '\xDB', '\xAF',  // stuv
                        '\xC3', '\x1C', '\xB3',  // wxyz
                        '\xD3', '\x5D', '\xB7',  // 0123
                        '\xE3', '\x9E', '\xBB',  // 4567
                        '\xF3', '\xDF', '\xBF'}; // 89+/
  s = b64::encode(&v[0], v.size() * sizeof(v[0]));
  CHECK("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" == s);
}

TEST_CASE("encode padding", "[base64]")
{
  CHECK("TWFu" == b64::encode("Man", 3));
  CHECK("TWE=" == b64::encode("Ma", 2));
  CHECK("TQ==" == b64::encode("M", 1));
}

TEST_CASE("encode wikipedia examples", "[base64]")
{
  CHECK("YW55IGNhcm5hbCBwbGVhc3VyZS4=" == b64::encode("any carnal pleasure.", 20));
  CHECK("YW55IGNhcm5hbCBwbGVhc3VyZQ==" == b64::encode("any carnal pleasure", 19));
  CHECK("YW55IGNhcm5hbCBwbGVhc3Vy" == b64::encode("any carnal pleasur", 18));
  CHECK("YW55IGNhcm5hbCBwbGVhc3U=" == b64::encode("any carnal pleasu", 17));
  CHECK("YW55IGNhcm5hbCBwbGVhcw==" == b64::encode("any carnal pleas", 16));
  CHECK("cGxlYXN1cmUu" == b64::encode("pleasure.", 9));
  CHECK("bGVhc3VyZS4=" == b64::encode("leasure.", 8));
  CHECK("ZWFzdXJlLg==" == b64::encode("easure.", 7));
  CHECK("YXN1cmUu" == b64::encode("asure.", 6));
  CHECK("c3VyZS4=" == b64::encode("sure.", 5));
}
