#ifndef CHANNEL_H
#define CHANNEL_H

#include "sndmix/audiochannel.h"
#include "sndmix/sourcenode.h"
#include "sndmix/sound.h"

namespace mt::snd {

class Channel : public SourceNode,
                public AudioChannel
{
public:
  void put_data(std::span<float> output_buffer,
                const PaStreamCallbackTimeInfo& time_info,
                std::size_t num_output_channels) override;

  void play(Sound s) override;

private:
  std::optional<std::pair<std::optional<std::size_t>, Sound>> playing_sound;
};

}  // namespace mt::snd

#endif /* CHANNEL_H */
