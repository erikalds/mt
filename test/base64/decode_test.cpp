#include "base64/decode.h"

#include <array>
#include <catch2/catch.hpp>

TEST_CASE("decode all", "[base64]")
{
  std::array<char, 3> buf3{ 0, 0, 0 };
  b64::decode("ABCD", &buf3[0], buf3.size());
  CHECK(buf3[0] == '\x00');
  CHECK(buf3[1] == '\x10');
  CHECK(buf3[2] == '\x83');

  std::array<char, 6> buf6{ 0, 0, 0, 0, 0, 0 };
  b64::decode("ABCDEFGH", &buf6[0], buf6.size());
  CHECK(buf6[0] == '\x00');
  CHECK(buf6[1] == '\x10');
  CHECK(buf6[2] == '\x83');
  CHECK(buf6[3] == '\x10');
  CHECK(buf6[4] == '\x51');
  CHECK(buf6[5] == '\x87');

  std::array<char, 48> buf48{ 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0 };
  b64::decode("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
              &buf48[0], buf48.size());
  std::size_t i = 0;
  CHECK('\x00' == buf48.at(i++));
  CHECK('\x10' == buf48.at(i++));
  CHECK('\x83' == buf48.at(i++));
  CHECK('\x10' == buf48.at(i++));
  CHECK('\x51' == buf48.at(i++));
  CHECK('\x87' == buf48.at(i++));
  CHECK('\x20' == buf48.at(i++));
  CHECK('\x92' == buf48.at(i++));
  CHECK('\x8B' == buf48.at(i++));
  CHECK('\x30' == buf48.at(i++));
  CHECK('\xD3' == buf48.at(i++));
  CHECK('\x8F' == buf48.at(i++));
  CHECK('\x41' == buf48.at(i++));
  CHECK('\x14' == buf48.at(i++));
  CHECK('\x93' == buf48.at(i++));
  CHECK('\x51' == buf48.at(i++));
  CHECK('\x55' == buf48.at(i++));
  CHECK('\x97' == buf48.at(i++));
  CHECK('\x61' == buf48.at(i++));
  CHECK('\x96' == buf48.at(i++));
  CHECK('\x9B' == buf48.at(i++));
  CHECK('\x71' == buf48.at(i++));
  CHECK('\xD7' == buf48.at(i++));
  CHECK('\x9F' == buf48.at(i++));
  CHECK('\x82' == buf48.at(i++));
  CHECK('\x18' == buf48.at(i++));
  CHECK('\xA3' == buf48.at(i++));
  CHECK('\x92' == buf48.at(i++));
  CHECK('\x59' == buf48.at(i++));
  CHECK('\xA7' == buf48.at(i++));
  CHECK('\xA2' == buf48.at(i++));
  CHECK('\x9A' == buf48.at(i++));
  CHECK('\xAB' == buf48.at(i++));
  CHECK('\xB2' == buf48.at(i++));
  CHECK('\xDB' == buf48.at(i++));
  CHECK('\xAF' == buf48.at(i++));
  CHECK('\xC3' == buf48.at(i++));
  CHECK('\x1C' == buf48.at(i++));
  CHECK('\xB3' == buf48.at(i++));
  CHECK('\xD3' == buf48.at(i++));
  CHECK('\x5D' == buf48.at(i++));
  CHECK('\xB7' == buf48.at(i++));
  CHECK('\xE3' == buf48.at(i++));
  CHECK('\x9E' == buf48.at(i++));
  CHECK('\xBB' == buf48.at(i++));
  CHECK('\xF3' == buf48.at(i++));
  CHECK('\xDF' == buf48.at(i++));
  CHECK('\xBF' == buf48.at(i++));
}

TEST_CASE("decode padding", "[base64]")
{
  SECTION("Man")
  {
    std::array<char, 4> buf{};
    b64::decode("TWFu", &buf[0], buf.size());
    CHECK(std::string_view{"Man"} == &buf[0]);
  }

  SECTION("Ma")
  {
    std::array<char, 3> buf{};
    b64::decode("TWE=", &buf[0], buf.size());
    CHECK(std::string_view{"Ma"} == &buf[0]);
  }

  SECTION("M")
  {
    std::array<char, 2> buf{};
    b64::decode("TQ==", &buf[0], buf.size());
    CHECK(std::string_view{"M"} == &buf[0]);
  }
}

TEST_CASE("decode wikipedia examples", "[base64]")
{
  SECTION("20 chars")
  {
    std::array<char, 21> buf{};
    b64::decode("YW55IGNhcm5hbCBwbGVhc3VyZS4=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasure.");
  }
  SECTION("19 chars")
  {
    std::array<char, 20> buf{};
    b64::decode("YW55IGNhcm5hbCBwbGVhc3VyZQ==", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasure");
  }
  SECTION("18 chars")
  {
    std::array<char, 19> buf{};
    b64::decode("YW55IGNhcm5hbCBwbGVhc3Vy", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasur");
  }
  SECTION("17 chars")
  {
    std::array<char, 18> buf{};
    b64::decode("YW55IGNhcm5hbCBwbGVhc3U=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasu");
  }
  SECTION("16 chars")
  {
    std::array<char, 17> buf{};
    b64::decode("YW55IGNhcm5hbCBwbGVhcw==", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleas");
  }

  SECTION("9 chars")
  {
    std::array<char, 10> buf{};
    b64::decode("cGxlYXN1cmUu", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "pleasure.");
  }
  SECTION("8 chars")
  {
    std::array<char, 9> buf{};
    b64::decode("bGVhc3VyZS4=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "leasure.");
  }
  SECTION("7 chars")
  {
    std::array<char, 8> buf{};
    b64::decode("ZWFzdXJlLg==", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "easure.");
  }
  SECTION("6 chars")
  {
    std::array<char, 7> buf{};
    b64::decode("YXN1cmUu", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "asure.");
  }
  SECTION("5 chars")
  {
    std::array<char, 6> buf{};
    b64::decode("c3VyZS4=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "sure.");
  }
}

TEST_CASE("data_size", "[base64]")
{
  CHECK(b64::data_size("TWFu") == 3U);
  CHECK(b64::data_size("TWE=") == 2U);
  CHECK(b64::data_size("TQ==") == 1U);
  CHECK(b64::data_size("cGxlYXN1cmUu") == 9U);
  CHECK(b64::data_size("bGVhc3VyZS4=") == 8U);
  CHECK(b64::data_size("ZWFzdXJlLg==") == 7U);
}

TEST_CASE("Ignores whitespace", "[base64]")
{
  SECTION("space")
  {
    std::array<char, 21> buf{};
    CHECK(b64::data_size("YW55IGNh  cm5hbC  BwbGV hc3Vy  ZS4=") == 20);
    b64::decode("YW55IGNh  cm5hbC  BwbGV  hc3Vy  ZS4=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasure.");
  }
  SECTION("tabs")
  {
    std::array<char, 21> buf{};
    CHECK(b64::data_size("YW55IGNh\tcm5hbC\tBwbGV hc3Vy\tZS4=") == 20);
    b64::decode("YW55IGNh\tcm5hbC\tBwbGV\thc3Vy\tZS4=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasure.");
  }
  SECTION("newlines")
  {
    std::array<char, 21> buf{};
    CHECK(b64::data_size("YW55IGNh\ncm5hbC\r\nBwbGV hc3Vy\r\nZS4=") == 20);
    b64::decode("YW55IGNh\ncm5hbC\nBwbGV\r\nhc3Vy\r\nZS4=", &buf[0], buf.size());
    CHECK(std::string_view{&buf[0]} == "any carnal pleasure.");
  }
}
