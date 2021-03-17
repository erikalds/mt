#ifndef SAMPLE_H
#define SAMPLE_H

#include <SFML/Audio/SoundBuffer.hpp>
#include <bits/c++config.h>
#include <memory>
#include <string>

namespace sf { class Sound; }

class Sample
{
public:
  Sample(std::string name_, void* pcm_data, std::size_t data_size);

  std::unique_ptr<sf::Sound> create_sound() const;
  std::string_view name() const { return sample_name; }

private:
  sf::SoundBuffer sound_buffer;
  std::string sample_name;
};


#endif /* SAMPLE_H */
