#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <memory>
#include <optional>

namespace mt::snd {

class AudioStream;
class Mixer;

class AudioSystem
{
public:
  explicit AudioSystem(std::optional<int> device_index);
  AudioSystem(const AudioSystem&) = delete;
  AudioSystem& operator=(const AudioSystem&) = delete;
  AudioSystem(AudioSystem&&) = delete;
  AudioSystem& operator=(AudioSystem&&) = delete;
  ~AudioSystem();

  std::unique_ptr<Mixer> create_mixer();

private:
  void list_devices() const;
  void open_output(std::optional<int> device_index);

  std::unique_ptr<AudioStream> stream;
};

} // namespace mt::sound

#endif /* AUDIOSYSTEM_H */
