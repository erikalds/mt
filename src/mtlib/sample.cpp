#include "sample.h"

#include <spdlog/spdlog.h>
#include <SFML/Audio/Sound.hpp>

namespace mt {

  Sample::Sample(std::string name_, void* pcm_data, std::size_t data_size,
                 float pitch_offset_) :
    sample_name{std::move(name_)},
    pitch_offset{pitch_offset_}
  {
    if (!sound_buffer.loadFromMemory(pcm_data, data_size))
    {
      spdlog::error("Failed to load PCM data for sample: {}", name());
    }
    else
    {
      spdlog::debug("Successfully loaded PCM data for sample: {}", name());
    }
  }

  std::unique_ptr<sf::Sound> Sample::create_sound() const
  {
    auto s = std::make_unique<sf::Sound>();
    s->setBuffer(sound_buffer);
    return s;
  }

}  // namespace mt
