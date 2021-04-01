#include "mtlib/wav_file_format.h"

#include <catch2/catch.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <cstdint>

#include <iostream>


TEST_CASE("one_waveform", "[wav]")
{
  sf::SoundBuffer sb{};
  std::array<std::int16_t, 8> samples{ 0, 500, 1000, 500, 0, -500, -1000, -500 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  sb.loadFromSamples(&samples[0], samples.size(), channelCount, sampleRate);
  mt::WavFileFormat wff{sb};
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
  sf::SoundBuffer sb{};
  std::array<std::int16_t, 8> samples{ 0, 16000, 32000, 16000, 0, -16000, -32000, -16000 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  sb.loadFromSamples(&samples[0], samples.size(), channelCount, sampleRate);
  mt::WavFileFormat wff{sb};
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
  sf::SoundBuffer sb{};
  std::array<std::int16_t, 16> samples{ 0, 16000, 32000, 16000, 0, -16000, -32000, -16000,
                                        0, 16000, 32000, 16000, 0, -16000, -32000, -16000 };
  const std::uint16_t channelCount{1};
  const std::uint32_t sampleRate{44100};
  sb.loadFromSamples(&samples[0], samples.size(), channelCount, sampleRate);
  mt::WavFileFormat wff{sb};
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
  sf::SoundBuffer sb{};
  std::array<std::int16_t, 16> samples{ 0, 0, 16000, 16000, 32000, 32000,
                                        16000, 16000, 0, 0, -16000, -16000,
                                        -32000, -32000, -16000, -16000 };
  const std::uint16_t channelCount{2};
  const std::uint32_t sampleRate{44100};
  sb.loadFromSamples(&samples[0], samples.size(), channelCount, sampleRate);
  mt::WavFileFormat wff{sb};
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
  sf::SoundBuffer sb{};
  std::array<std::int16_t, 16> samples{ 0, 0, 16000, 16000, 32000, 32000,
                                        16000, 16000, 0, 0, -16000, -16000,
                                        -32000, -32000, -16000, -16000 };
  const std::uint16_t channelCount{2};
  const std::uint32_t sampleRate{22050};
  sb.loadFromSamples(&samples[0], samples.size(), channelCount, sampleRate);
  mt::WavFileFormat wff{sb};
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
