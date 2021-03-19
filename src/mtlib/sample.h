#ifndef SAMPLE_H
#define SAMPLE_H

#include <SFML/Audio/SoundBuffer.hpp>
#include <bits/c++config.h>
#include <memory>
#include <string>

namespace sf { class Sound; }

namespace mt {

  class Sample
  {
  public:
    Sample(std::string name_, void* pcm_data, std::size_t data_size,
           float pitch_offset_);

    std::unique_ptr<sf::Sound> create_sound() const;

    std::string_view name() const { return sample_name; }
    void set_name(std::string_view n) { sample_name = n; }

    float get_pitch_offset() const { return pitch_offset; }

  private:
    sf::SoundBuffer sound_buffer;
    std::string sample_name;
    float pitch_offset;
  };

}  // namespace mt

#endif /* SAMPLE_H */
