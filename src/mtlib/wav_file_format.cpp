#include "wav_file_format.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <cassert>
#include <cstdint>

namespace mt {

  // sf::SoundBuffer only supports 16-bit samples
  WavFileFormat::WavFileFormat(const sf::SoundBuffer& sb) :
    data(44 + sb.getSampleCount() * 2, 0)
  {
    std::size_t pos{0};
    this->write_at_pos(pos, "RIFF"); // main chunk ID
    assert(data.size() - 8U < std::numeric_limits<std::uint32_t>::max());
    const std::uint32_t main_chunk_size{static_cast<std::uint32_t>(data.size() - 8U)};
    this->write_at_pos(pos, main_chunk_size);
    this->write_at_pos(pos, "WAVE"); // main chunk format ID
    this->write_at_pos(pos, "fmt "); // sub-chunk 1 ("format") ID
    const std::uint32_t format_chunk_size{16};
    this->write_at_pos(pos, format_chunk_size);
    this->write_at_pos(pos, static_cast<std::uint16_t>(1)); // format (PCM)
    const auto channel_count{static_cast<std::uint16_t>(sb.getChannelCount())};
    this->write_at_pos(pos, channel_count);
    const std::uint32_t sample_rate = sb.getSampleRate();
    this->write_at_pos(pos, sample_rate);
    const std::uint16_t bytes_per_sample{2};
    const std::uint32_t byte_rate{sample_rate * channel_count * bytes_per_sample};
    this->write_at_pos(pos, byte_rate);
    const auto block_align{static_cast<std::uint16_t>(channel_count * bytes_per_sample)};
    this->write_at_pos(pos, block_align);
    const std::uint16_t bits_per_sample = bytes_per_sample * 8;
    this->write_at_pos(pos, bits_per_sample);
    this->write_at_pos(pos, "data");

    assert(sb.getSampleCount() < std::numeric_limits<std::uint32_t>::max());
    auto sample_count = static_cast<std::uint32_t>(sb.getSampleCount());
    this->write_at_pos(pos, sample_count * bytes_per_sample);

    for(std::size_t i = 0; i < sample_count; ++i)
    {
      this->write_at_pos(pos, sb.getSamples()[i]);
    }
 }

  void WavFileFormat::write_at_pos(std::size_t& i, std::string_view val)
  {
    for (auto c : val)
    {
      data[i++] = c;
    }
  }

  void WavFileFormat::write_at_pos(std::size_t& i, std::uint32_t val)
  {
    write_integral_at_pos(i, val);
  }

  void WavFileFormat::write_at_pos(std::size_t& i, std::uint16_t val)
  {
    write_integral_at_pos(i, val);
  }

  void WavFileFormat::write_at_pos(std::size_t& i, std::int16_t val)
  {
    write_integral_at_pos(i, val);
  }

  // Write val in data (little endian) at position i. The reference i is
  // incremented to point past the last byte of val written to data.
  template<typename T>
  void WavFileFormat::write_integral_at_pos(std::size_t& i, T val)
  {
    constexpr const std::size_t bytesize{8};
    constexpr const std::size_t bits{sizeof(val) * bytesize};
    constexpr const T bytemask{0xFF};
    for (std::size_t j = 0; j < bits; j += bytesize)
    {
      data[i++] = static_cast<char>((val & (bytemask << j)) >> j);
    }
  }

}  // namespace mt
