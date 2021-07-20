#ifndef CHANNEL_H
#define CHANNEL_H

#include "sndmix/sourcenode.h"

namespace mt::snd {

class Channel : public SourceNode
{
public:
  void put_data(std::span<float> output_buffer,
                const PaStreamCallbackTimeInfo& time_info,
                std::size_t num_output_channels) override;
};

}  // namespace mt::snd

#endif /* CHANNEL_H */
