#ifndef WAV_FILE_FORMAT_H
#define WAV_FILE_FORMAT_H

#include <string_view>
#include <vector>

namespace sf { class SoundBuffer; }

namespace mt
{

  class WavFileFormat
  {
  public:
    explicit WavFileFormat(const sf::SoundBuffer& sb);

    explicit operator const void*() const { return &data[0]; }
    [[nodiscard]] std::size_t size() const { return data.size(); }

  private:
    void write_at_pos(std::size_t& i, std::string_view val);
    void write_at_pos(std::size_t& i, std::uint32_t val);
    void write_at_pos(std::size_t& i, std::uint16_t val);
    void write_at_pos(std::size_t& i, std::int16_t val);

    template<typename T>
    void write_integral_at_pos(std::size_t& i, T val);

    std::vector<char> data;
  };

}  // namespace mt

#endif /* WAV_FILE_FORMAT_H */
