#ifndef sndmix_mixer_h
#define sndmix_mixer_h

#include <memory>
#include <vector>

namespace mt::snd {

  class AudioChannel;
  class Sound;

  class Mixer
  {
  public:
    explicit Mixer(std::vector<std::unique_ptr<AudioChannel>> channels_);
    Mixer(const Mixer&) = delete;
    Mixer& operator=(const Mixer&) = delete;
    Mixer(Mixer&&) = delete;
    Mixer& operator=(Mixer&&) = delete;
    ~Mixer();

    void play(Sound s);

  private:
    std::vector<std::unique_ptr<AudioChannel>> channels;
    std::size_t rr_index = 0;
  };

}  // namespace mt::snd

#endif /* sndmix_mixer_h */
