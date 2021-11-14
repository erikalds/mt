#include "mixer.h"

#include "audiochannel.h"
#include "sound.h"
#include <spdlog/spdlog.h>


namespace mt::snd {

  Mixer::Mixer(std::vector<std::unique_ptr<AudioChannel>> channels_) :
    channels{std::move(channels_)}
  {
  }

  Mixer::~Mixer() = default;

  void Mixer::play(Sound s)
  {
    channels.at(rr_index)->play(std::move(s));
    spdlog::debug("Playing sound at channel {}", rr_index);
    rr_index = (rr_index + 1) % channels.size();
  }

}  // namespace mt::snd
