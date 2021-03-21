#include "sample.h"

#include "audio_data_presenter.h"
#include "mtlib/sample_data_iterator.h"
#include <cstdint>
#include <spdlog/spdlog.h>
#include <SFML/Audio/Sound.hpp>

namespace mt {

  Sample::Sample(const std::filesystem::path& fname) :
    sample_name{fname.filename()},
    pitch_offset{0}
  {
    if (!sound_buffer.loadFromFile(fname.string()))
    {
      spdlog::error("Failed to load PCM data for sample: {}", name());
    }
    else
    {
      spdlog::debug("Successfully loaded PCM data for sample: {}", name());
    }
  }

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

  void Sample::present_audio_data(AudioDataPresenter& p) const
  {
    p.present_details(sound_buffer.getSampleRate(), sound_buffer.getDuration());
    for (auto channel = 0U; channel < sound_buffer.getChannelCount(); ++channel)
    {
      p.present_channel(sample_data_iterator<const std::int16_t>{sound_buffer.getSamples(),
                                                                 sound_buffer.getSampleCount(),
                                                                 sound_buffer.getChannelCount(),
                                                                 channel},
                        sample_data_iterator<const std::int16_t>{});
    }
  }

}  // namespace mt
