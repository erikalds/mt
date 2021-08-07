#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>

namespace mt::snd {

  class Sound;

  class SoundBuffer
  {
  public:
    SoundBuffer() = default;
    SoundBuffer(const SoundBuffer&);
    SoundBuffer& operator=(const SoundBuffer&);
    SoundBuffer(SoundBuffer&&) = default;
    SoundBuffer& operator=(SoundBuffer&&) = default;
    SoundBuffer(std::unique_ptr<std::int16_t[]> samples_,
                std::uint32_t sample_count_,
                std::uint16_t channel_count_,
                std::uint32_t sample_rate_);
    ~SoundBuffer() = default;

    [[nodiscard]] const std::int16_t* get_samples() const;
    [[nodiscard]] const float* get_float_samples() const;
    [[nodiscard]] std::uint32_t get_sample_count() const;
    [[nodiscard]] std::uint32_t get_sample_rate() const;
    [[nodiscard]] std::chrono::microseconds get_duration() const;
    [[nodiscard]] std::uint16_t get_channel_count() const;

    // [[nodiscard]] std::unique_ptr<Sound> create_sound() const;

  private:
    std::unique_ptr<std::int16_t[]> samples = nullptr;
    std::unique_ptr<float[]> fsamples = nullptr;
    std::uint32_t sample_count = 0;
    std::uint32_t sample_rate = 0;
    std::uint16_t channel_count = 0;
  };

}  // namespace mt::snd

#endif /* SOUNDBUFFER_H */
