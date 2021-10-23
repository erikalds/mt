#include "wav_file_format.h"

#include <cassert>
#include <cstdint>
#include <limits>

namespace mt {

  constexpr static const auto hdrsize = 44U;

  // sf::SoundBuffer only supports 16-bit samples
  WavFileFormat::WavFileFormat(const std::int16_t* samples,
                               std::uint32_t sample_count,
                               std::uint16_t channel_count,
                               std::uint32_t sample_rate) :
    data(hdrsize + sample_count * 2, 0)
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
    this->write_at_pos(pos, channel_count);
    this->write_at_pos(pos, sample_rate);
    const std::uint16_t bytes_per_sample{2};
    const std::uint32_t byte_rate{sample_rate * channel_count * bytes_per_sample};
    this->write_at_pos(pos, byte_rate);
    const auto block_align{static_cast<std::uint16_t>(channel_count * bytes_per_sample)};
    this->write_at_pos(pos, block_align);
    const std::uint16_t bits_per_sample = bytes_per_sample * 8;
    this->write_at_pos(pos, bits_per_sample);
    this->write_at_pos(pos, "data");

    this->write_at_pos(pos, sample_count * bytes_per_sample);

    for(std::size_t i = 0; i < sample_count; ++i)
    {
      this->write_at_pos(pos, samples[i]);
    }
  }

  namespace {

    const auto bits_per_byte{8U};

    template<typename T>
    constexpr T read_at_pos(const void* mem, std::size_t pos)
    {
      T val{0};
      const T bytemask{0xFF};
      for (std::size_t i = 0; i < sizeof(T); ++i)
      {
        const T byte = static_cast<T>(*(static_cast<const char*>(mem) + pos++)) & bytemask;
        val += static_cast<T>(byte << (i * bits_per_byte));
      }
      return val;
    }

    template<int inc=1>
    constexpr void increment(const void** mem)
    {
      *mem = static_cast<const char*>(*mem) + inc;
    }

    template<typename T>
    constexpr T read(const void** mem)
    {
      T val = read_at_pos<T>(*mem, 0);
      increment<sizeof(T)>(mem);
      return val;
    }

    // We always read four bytes when reading strings from WAV format
    template<>
    constexpr std::string_view read<std::string_view>(const void** mem)
    {
      const auto* char_data = static_cast<const char*>(*mem);
      increment<4>(mem);
      return {char_data, 4};
    }
  }  // anonymous namespace

  std::optional<WavFileFormat> WavFileFormat::load_from_memory(const void* mem,
                                                               std::size_t size)
  {
#define RETURN_IF(boolean_expr) if ((boolean_expr)) return {} // NOLINT

    RETURN_IF("RIFF" != read<std::string_view>(&mem)); // 0 0
    auto main_chunk_size = read<std::uint32_t>(&mem); // 4 4
    const auto WAVE_pos = 8;
    RETURN_IF(main_chunk_size + WAVE_pos != size);

    RETURN_IF("WAVE" != read<std::string_view>(&mem)); // 8 8
    RETURN_IF("fmt " != read<std::string_view>(&mem)); // 12 c
    auto format_chunk_size = read<std::uint32_t>(&mem); // 16 10
    const void* data = static_cast<const char*>(mem) + format_chunk_size;
    auto format = read<std::uint16_t>(&mem); // 20 14
    RETURN_IF(format != 1); // we only support PCM data
    auto channel_count = read<std::uint16_t>(&mem); // 22 16
    auto sample_rate = read<std::uint32_t>(&mem); // 24 18
    /*auto byte_rate =*/ read<std::uint32_t>(&mem); // 28 1c
    /*auto block_align =*/ read<std::uint16_t>(&mem); // 32 20
    auto bits_per_sample = read<std::uint16_t>(&mem); // 34 22

    RETURN_IF("data" != read<std::string_view>(&data)); // 36 24 (iff format_chunk_size == 16)
    auto data_size = read<std::uint32_t>(&data); // 40 28
    auto bytes_per_sample = bits_per_sample / bits_per_byte;
    auto sample_count = data_size / bytes_per_sample;
    return WavFileFormat{static_cast<const std::int16_t*>(data), // 44 2c
                         sample_count, channel_count, sample_rate};
#undef RETURN_IF
  }

  [[nodiscard]] std::uint16_t WavFileFormat::channel_count() const
  {
    const auto channel_count_pos{22};
    return read_at_pos<std::uint16_t>(&data[0], channel_count_pos);
  }

  [[nodiscard]] std::uint32_t WavFileFormat::sample_rate() const
  {
    const auto sample_rate_pos{24};
    return read_at_pos<std::uint32_t>(&data[0], sample_rate_pos);
  }

  [[nodiscard]] std::uint32_t WavFileFormat::sample_count() const
  {
    const auto sample_count_pos{40};
    const auto bytes_per_sample{2U};
    return read_at_pos<std::uint32_t>(&data[0], sample_count_pos) / bytes_per_sample;
  }

  [[nodiscard]] const std::int16_t* WavFileFormat::samples() const
  {
    const auto data_pos{44};
    const void* ptr = &data[data_pos];
    return static_cast<const std::int16_t*>(ptr);
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
