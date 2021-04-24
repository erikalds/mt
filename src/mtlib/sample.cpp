#include "sample.h"

#include "mtlib/audio_data_presenter.h"
#include "mtlib/sample_data_iterator.h"
#include "mtlib/wav_file_format.h"
#include "base64/decode.h"
#include "base64/encode.h"
#include <cstdint>
#include <spdlog/spdlog.h>
#include <SFML/Audio/Sound.hpp>
#include <yaml-cpp/yaml.h>

namespace mt {

  Sample::Sample(const std::filesystem::path& fname) :
    sample_name{fname.filename().string()},
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

  YAML::Node Sample::get_as_yaml() const
  {
    YAML::Node node{};
    node["name"] = this->sample_name;
    node["pitch-offset"] = this->pitch_offset;

    assert(this->sound_buffer.getSampleCount() < std::numeric_limits<std::uint32_t>::max());
    assert(this->sound_buffer.getChannelCount() < std::numeric_limits<std::uint16_t>::max());
    auto sc = static_cast<std::uint32_t>(this->sound_buffer.getSampleCount());
    auto cc = static_cast<std::uint16_t>(this->sound_buffer.getChannelCount());
    WavFileFormat wff{this->sound_buffer.getSamples(), sc, cc,
                      this->sound_buffer.getSampleRate()};
    node["pcm-data"] = b64::encode(static_cast<const void*>(wff), wff.size());

    return node;
  }

  Sample Sample::load_from_yaml(const YAML::Node& node)
  {
    auto b64data = node["pcm-data"].as<std::string>();
    std::vector<unsigned char> pcm_data;
    pcm_data.resize(b64::data_size(b64data.c_str()), 0);
    b64::decode(b64data.c_str(),
                &pcm_data[0], pcm_data.size());
    return Sample{node["name"].as<std::string>(),
                  &pcm_data[0], pcm_data.size(),
                  node["pitch-offset"].as<float>()};
  }

}  // namespace mt
