#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include "sndmix/sinknode.h"

struct PaStreamCallbackTimeInfo;

namespace mt::snd {

class AudioMixer : public SinkNode
{
public:
  explicit AudioMixer(unsigned int output_channels_);

  void write_out(float* output_buffer, unsigned long frames_per_buffer,
                 const PaStreamCallbackTimeInfo& time_info);

private:
  unsigned int output_channels;
};

}  // namespace mt::snd

#endif /* AUDIOMIXER_H */
