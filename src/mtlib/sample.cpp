#include "sample.h"

#include "mtlib/audio_data_presenter.h"
#include "sndmix/sample_data_iterator.h"
#include "mtlib/wav_file_format.h"
#include "sndmix/soundbuffer.h"
#include "base64/decode.h"
#include "base64/encode.h"
#include <cstdint>
#include <fstream>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

namespace mt {

  Sample::Sample(const std::filesystem::path& fname) :
    sample_name{fname.filename().string()},
    pitch_offset{0}
  {
    std::ifstream in(fname, std::ios::binary | std::ios::ate);
    const auto ssize = in.tellg();
    const auto size = static_cast<std::size_t>(ssize);
    in.seekg(0, std::ios::beg);
    std::vector<char> data(size);
    if (in.read(data.data(), ssize))
    {
      spdlog::debug("Successfully read file {} [{} bytes]", fname.string(), size);
      load_sound_buffer(data.data(), size);
    }
    else
    {
      spdlog::error("Failed to read file {} [{} bytes]", fname.string(), size);
    }
  }

  Sample::Sample(std::string name_, void* pcm_data, std::size_t data_size,
                 float pitch_offset_) :
    sample_name{std::move(name_)},
    pitch_offset{pitch_offset_}
  {
    load_sound_buffer(pcm_data, data_size);
  }

  // std::unique_ptr<sf::Sound> Sample::create_sound() const
  // {
  //   auto s = std::make_unique<sf::Sound>();
  //   s->setBuffer(sound_buffer);
  //   return s;
  // }

  void Sample::present_audio_data(AudioDataPresenter& p) const
  {
    p.present_details(sound_buffer.get_sample_rate(), sound_buffer.get_duration());
    for (auto channel = 0U; channel < sound_buffer.get_channel_count(); ++channel)
    {
      p.present_channel(snd::sample_data_iterator<const std::int16_t>{sound_buffer.get_samples(),
                                                                      sound_buffer.get_sample_count(),
                                                                      sound_buffer.get_channel_count(),
                                                                      channel},
                        snd::sample_data_iterator<const std::int16_t>{});
    }
  }

  YAML::Node Sample::get_as_yaml() const
  {
    YAML::Node node{};
    node["name"] = this->sample_name;
    node["pitch-offset"] = this->pitch_offset;

    WavFileFormat wff{this->sound_buffer.get_samples(),
                      this->sound_buffer.get_sample_count(),
                      this->sound_buffer.get_channel_count(),
                      this->sound_buffer.get_sample_rate()};
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

  void Sample::load_sound_buffer(const void* pcm_data, std::size_t data_size)
  {
    auto wff = WavFileFormat::load_from_memory(pcm_data, data_size);
    if (wff)
    {
      std::unique_ptr<std::int16_t[]> samples(new std::int16_t[data_size]);
      std::copy(wff->samples(), wff->samples() + wff->sample_count(),
                samples.get());
      spdlog::debug("Successfully loaded PCM data for sample: {}", name());
      sound_buffer = snd::SoundBuffer(std::move(samples), wff->sample_count(),
                                      wff->channel_count(), wff->sample_rate());
    }
    else
    {
      spdlog::error("Failed to load PCM data for sample: {}. Unsupported format.", name());
    }
  }

}  // namespace mt
