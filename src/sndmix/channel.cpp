#include "channel.h"

#include "sound.h"

namespace mt::snd {

void Channel::put_data(std::span<float> output_buffer,
                       const PaStreamCallbackTimeInfo& /*time_info*/,
                       std::size_t num_output_channels)
{
  if (!playing_sound
      || !playing_sound->first)
  {
    return;
  }

  const int sample_rate{44100};
  playing_sound->first = playing_sound->second.write_to(output_buffer,
                                                        num_output_channels,
                                                        sample_rate,
                                                        *playing_sound->first);
}

void Channel::play(Sound s)
{
  playing_sound = std::make_pair(0, s);
}

}  // namespace mt::snd
