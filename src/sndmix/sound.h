#ifndef SOUND_H
#define SOUND_H

#include <cstdint>
#include <optional>
#include <span>

namespace mt::snd {

  class SoundBuffer;

  class Sound
  {
  public:
    Sound(const SoundBuffer& buf_, double pitch_);
    Sound(const Sound&) = default;
    Sound& operator=(const Sound&) = default;
    Sound(Sound&&) = default;
    Sound& operator=(Sound&&) = default;
    ~Sound() = default;

    std::optional<std::size_t> write_to(std::span<float> output_buffer,
                                        std::size_t output_channels,
                                        std::uint32_t output_sample_rate,
                                        std::size_t offset);

  private:
    const SoundBuffer* buf;
    double pitch;
  };

}  // namespace mt::snd

#endif /* SOUND_H */
