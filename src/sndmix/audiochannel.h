#ifndef AUDIOCHANNEL_H
#define AUDIOCHANNEL_H

namespace mt::snd {

  class Sound;

  class AudioChannel
  {
  public:
    virtual ~AudioChannel() = 0;

    virtual void play(Sound s) = 0;
  };

  inline AudioChannel::~AudioChannel() = default;

}  // namespace mt::snd

#endif /* AUDIOCHANNEL_H */
