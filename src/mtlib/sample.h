#ifndef SAMPLE_H
#define SAMPLE_H

#include <SFML/Audio/SoundBuffer.hpp>
#include <bits/c++config.h>
#include <filesystem>
#include <memory>
#include <string>

namespace sf { class Sound; }
namespace YAML { class Node; }

namespace mt {

  class AudioDataPresenter;

  class Sample
  {
  public:
    explicit Sample(const std::filesystem::path& fname);
    Sample(std::string name_, void* pcm_data, std::size_t data_size,
           float pitch_offset_);

    std::unique_ptr<sf::Sound> create_sound() const;

    std::string_view name() const { return sample_name; }
    void set_name(std::string_view n) { sample_name = n; }

    float get_pitch_offset() const { return pitch_offset; }

    void present_audio_data(AudioDataPresenter& p) const;

    YAML::Node get_as_yaml() const;
    [[nodiscard]] static Sample load_from_yaml(const YAML::Node& node);

  private:
    sf::SoundBuffer sound_buffer;
    std::string sample_name;
    float pitch_offset;
  };

}  // namespace mt

#endif /* SAMPLE_H */
