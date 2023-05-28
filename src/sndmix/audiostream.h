#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <memory>
#include <vector>

using PaStream = void;

namespace mt::snd {

class AudioChannel;
class AudioMixer;

class AudioStream
{
public:
  explicit AudioStream(int audio_device_index);
  AudioStream(const AudioStream&) = delete;
  AudioStream& operator=(const AudioStream&) = delete;
  AudioStream(AudioStream&&) = delete;
  AudioStream& operator=(AudioStream&&) = delete;
  ~AudioStream();

  std::vector<std::unique_ptr<AudioChannel>> create_channels(std::size_t count);

private:
  PaStream* stream = nullptr;
  std::unique_ptr<AudioMixer> mixer;
};

} // namespace mt::snd

#endif /* AUDIOSTREAM_H */