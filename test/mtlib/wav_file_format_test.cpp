#include "mtlib/wav_file_format.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

#include <iostream>


TEST_CASE("one_waveform", "[wav]")
{
  std::array<std::int16_t, 8> samples{ 0, 500, 1000, 500, 0, -500, -1000, -500 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  mt::WavFileFormat wff{&samples[0], samples.size(), channelCount, sampleRate};
  const auto* data = static_cast<const void*>(wff);
  const auto* cdata = static_cast<const char*>(data);
  const auto* u16data = static_cast<const std::uint16_t*>(data);
  const auto* i16data = static_cast<const std::int16_t*>(data);
  const auto* u32data = static_cast<const std::uint32_t*>(data);
  CHECK("RIFF" == std::string(&cdata[0], 4));
  CHECK(44U - 8U + samples.size() * 2 == u32data[4 / 4]); // main chunk size
  CHECK("WAVE" == std::string(&cdata[8], 4));
  CHECK("fmt " == std::string(&cdata[12], 4));
  CHECK(16U == u32data[16 / 4]); // fmt chunk size
  CHECK(1U == u16data[20 / 2]); // PCM format
  CHECK(channelCount == u16data[22 / 2]);
  CHECK(sampleRate == u32data[24 / 4]);
  CHECK(sampleRate * channelCount * 2 == u32data[28 / 4]); // byte rate
  CHECK(channelCount * 2 == u16data[32 / 2]); // block align
  CHECK(16 == u16data[34 / 2]); // bits per sample
  CHECK("data" == std::string(&cdata[36], 4));
  CHECK(samples.size() * 2 == u32data[40 / 4]); // data chunk size

  CHECK(44U + samples.size() * 2 == wff.size());

  for (auto i = 0U; i < samples.size(); ++i)
  {
    CHECK(samples[i] == i16data[44 / 2 + i]);
  }
}

TEST_CASE("another_waveform", "[wav]")
{
  std::array<std::int16_t, 8> samples{ 0, 16000, 32000, 16000, 0, -16000, -32000, -16000 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  mt::WavFileFormat wff{&samples[0], samples.size(), channelCount, sampleRate};
  const auto* data = static_cast<const void*>(wff);
  const auto* cdata = static_cast<const char*>(data);
  const auto* u16data = static_cast<const std::uint16_t*>(data);
  const auto* i16data = static_cast<const std::int16_t*>(data);
  const auto* u32data = static_cast<const std::uint32_t*>(data);
  CHECK("RIFF" == std::string(&cdata[0], 4));
  CHECK(44U - 8U + samples.size() * 2 == u32data[4 / 4]); // main chunk size
  CHECK("WAVE" == std::string(&cdata[8], 4));
  CHECK("fmt " == std::string(&cdata[12], 4));
  CHECK(16U == u32data[16 / 4]); // fmt chunk size
  CHECK(1U == u16data[20 / 2]); // PCM format
  CHECK(channelCount == u16data[22 / 2]);
  CHECK(sampleRate == u32data[24 / 4]);
  CHECK(sampleRate * channelCount * 2 == u32data[28 / 4]); // byte rate
  CHECK(channelCount * 2 == u16data[32 / 2]); // block align
  CHECK(16 == u16data[34 / 2]); // bits per sample
  CHECK("data" == std::string(&cdata[36], 4));
  CHECK(samples.size() * 2 == u32data[40 / 4]); // data chunk size

  CHECK(44U + samples.size() * 2 == wff.size());

  for (auto i = 0U; i < samples.size(); ++i)
  {
    CHECK(samples[i] == i16data[44 / 2 + i]);
  }
}

TEST_CASE("longer_waveform", "[wav]")
{
  std::array<std::int16_t, 16> samples{ 0, 16000, 32000, 16000, 0, -16000, -32000, -16000,
                                        0, 16000, 32000, 16000, 0, -16000, -32000, -16000 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  mt::WavFileFormat wff{&samples[0], samples.size(), channelCount, sampleRate};
  const auto* data = static_cast<const void*>(wff);
  const auto* cdata = static_cast<const char*>(data);
  const auto* u16data = static_cast<const std::uint16_t*>(data);
  const auto* i16data = static_cast<const std::int16_t*>(data);
  const auto* u32data = static_cast<const std::uint32_t*>(data);
  CHECK("RIFF" == std::string(&cdata[0], 4));
  CHECK(44U - 8U + samples.size() * 2 == u32data[4 / 4]); // main chunk size
  CHECK("WAVE" == std::string(&cdata[8], 4));
  CHECK("fmt " == std::string(&cdata[12], 4));
  CHECK(16U == u32data[16 / 4]); // fmt chunk size
  CHECK(1U == u16data[20 / 2]); // PCM format
  CHECK(channelCount == u16data[22 / 2]);
  CHECK(sampleRate == u32data[24 / 4]);
  CHECK(sampleRate * channelCount * 2 == u32data[28 / 4]); // byte rate
  CHECK(channelCount * 2 == u16data[32 / 2]); // block align
  CHECK(16 == u16data[34 / 2]); // bits per sample
  CHECK("data" == std::string(&cdata[36], 4));
  CHECK(samples.size() * 2 == u32data[40 / 4]); // data chunk size

  CHECK(44U + samples.size() * 2 == wff.size());

  for (auto i = 0U; i < samples.size(); ++i)
  {
    CHECK(samples[i] == i16data[44 / 2 + i]);
  }
}

TEST_CASE("stereo_sample", "[wav]")
{
  std::array<std::int16_t, 16> samples{ 0, 0, 16000, 16000, 32000, 32000,
                                        16000, 16000, 0, 0, -16000, -16000,
                                        -32000, -32000, -16000, -16000 };
  const std::uint16_t channelCount{2};
  const std::uint32_t sampleRate{44100};
  mt::WavFileFormat wff{&samples[0], samples.size(), channelCount, sampleRate};
  const auto* data = static_cast<const void*>(wff);
  const auto* cdata = static_cast<const char*>(data);
  const auto* u16data = static_cast<const std::uint16_t*>(data);
  const auto* i16data = static_cast<const std::int16_t*>(data);
  const auto* u32data = static_cast<const std::uint32_t*>(data);
  CHECK("RIFF" == std::string(&cdata[0], 4));
  CHECK(44U - 8U + samples.size() * 2 == u32data[4 / 4]); // main chunk size
  CHECK("WAVE" == std::string(&cdata[8], 4));
  CHECK("fmt " == std::string(&cdata[12], 4));
  CHECK(16U == u32data[16 / 4]); // fmt chunk size
  CHECK(1U == u16data[20 / 2]); // PCM format
  CHECK(channelCount == u16data[22 / 2]);
  CHECK(sampleRate == u32data[24 / 4]);
  CHECK(sampleRate * channelCount * 2 == u32data[28 / 4]); // byte rate
  CHECK(channelCount * 2 == u16data[32 / 2]); // block align
  CHECK(16 == u16data[34 / 2]); // bits per sample
  CHECK("data" == std::string(&cdata[36], 4));
  CHECK(samples.size() * 2 == u32data[40 / 4]); // data chunk size

  CHECK(44U + samples.size() * 2 == wff.size());

  for (auto i = 0U; i < samples.size(); ++i)
  {
    CHECK(samples[i] == i16data[44 / 2 + i]);
  }
}

TEST_CASE("different_sample_rate", "[wav]")
{
  std::array<std::int16_t, 16> samples{ 0, 0, 16000, 16000, 32000, 32000,
                                        16000, 16000, 0, 0, -16000, -16000,
                                        -32000, -32000, -16000, -16000 };
  const std::uint16_t channelCount{2};
  const std::uint32_t sampleRate{22050};
  mt::WavFileFormat wff{&samples[0], samples.size(), channelCount, sampleRate};
  const auto* data = static_cast<const void*>(wff);
  const auto* cdata = static_cast<const char*>(data);
  const auto* u16data = static_cast<const std::uint16_t*>(data);
  const auto* i16data = static_cast<const std::int16_t*>(data);
  const auto* u32data = static_cast<const std::uint32_t*>(data);
  CHECK("RIFF" == std::string(&cdata[0], 4));
  CHECK(44U - 8U + samples.size() * 2 == u32data[4 / 4]); // main chunk size
  CHECK("WAVE" == std::string(&cdata[8], 4));
  CHECK("fmt " == std::string(&cdata[12], 4));
  CHECK(16U == u32data[16 / 4]); // fmt chunk size
  CHECK(1U == u16data[20 / 2]); // PCM format
  CHECK(channelCount == u16data[22 / 2]);
  CHECK(sampleRate == u32data[24 / 4]);
  CHECK(sampleRate * channelCount * 2 == u32data[28 / 4]); // byte rate
  CHECK(channelCount * 2 == u16data[32 / 2]); // block align
  CHECK(16 == u16data[34 / 2]); // bits per sample
  CHECK("data" == std::string(&cdata[36], 4));
  CHECK(samples.size() * 2 == u32data[40 / 4]); // data chunk size

  CHECK(44U + samples.size() * 2 == wff.size());

  for (auto i = 0U; i < samples.size(); ++i)
  {
    CHECK(samples[i] == i16data[44 / 2 + i]);
  }
}

TEST_CASE("read_wav_format_one", "[wav]")
{
  std::array<std::int16_t, 8> samples{ 0, 500, 1000, 500, 0, -500, -1000, -500 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  mt::WavFileFormat source{&samples[0], samples.size(), channelCount, sampleRate};
  auto wff = mt::WavFileFormat::load_from_memory(static_cast<const void*>(source), source.size());
  REQUIRE(wff.has_value() == true);
  CHECK(wff->channel_count() == channelCount);
  CHECK(wff->sample_rate() == sampleRate);
  CHECK(wff->sample_count() == samples.size());
  for (auto i = 0U; i < samples.size(); ++i)
  {
    CHECK(samples[i] == wff->samples()[i]);
  }
}

namespace {
 //                      0   4
  const char* complete = "RIFF\x2c\x0\x0\x0"
  // 8   c  10           14          18            1c          20
    "WAVEfmt \x10\x0\x0\x0\x1\x0\x2\x0\x44\xac\x0\x0\x1\x0\x0\x0\x2\x0\x10\x0"
  //24  28           2c          30          34          38
    "data\x10\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\x0\x8\x0";
  const std::size_t complete_size{52};
} // anonymous namespace

TEST_CASE("fails_when_malformed_header", "[wav]")
{
  std::string wrong{complete, complete_size};
  wrong[0] = 'B';
  auto wff = mt::WavFileFormat::load_from_memory(&wrong[0], wrong.size());
  CHECK(wff.has_value() == false);

  wrong = std::string{complete, complete_size};
  wrong[11] = 'X';
  wff = mt::WavFileFormat::load_from_memory(&wrong[0], wrong.size());
  CHECK(wff.has_value() == false);

  wrong = std::string{complete, complete_size};
  wrong[15] = 'x';
  wff = mt::WavFileFormat::load_from_memory(&wrong[0], wrong.size());
  CHECK(wff.has_value() == false);

  wrong = std::string{complete, complete_size};
  wrong[20] = '\x0';
  wff = mt::WavFileFormat::load_from_memory(&wrong[0], wrong.size());
  CHECK(wff.has_value() == false);

  wrong = std::string{complete, complete_size};
  wrong[39] = 'x';
  wff = mt::WavFileFormat::load_from_memory(&wrong[0], wrong.size());
  CHECK(wff.has_value() == false);

  wff = mt::WavFileFormat::load_from_memory(complete, complete_size);
  CHECK(wff.has_value() == true);
}

TEST_CASE("fails_when_main_chunk_size_and_size_mismatch", "[wav]")
{
  auto wff = mt::WavFileFormat::load_from_memory(complete, complete_size + 1);
  CHECK(wff.has_value() == false);
}

TEST_CASE("Uses_format_chunk_size_to_find_data", "[wav]")
{
  std::string data{complete, complete_size};
  data = data.substr(0, 0x24) + std::string{"\x0\x0", 2} + data.substr(0x24);
  data[4] = '\x2e';
  data[0x10] = '\x12';
  auto wff = mt::WavFileFormat::load_from_memory(&data[0], data.size());
  CHECK(wff.has_value() == true);
}
