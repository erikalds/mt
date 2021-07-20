#include "audiomixer.h"

#include "sourcenode.h"

#include <spdlog/spdlog.h>
#include <array>
#include <span>

namespace mt::snd {

const auto max_channels{32};

AudioMixer::AudioMixer(unsigned int output_channels_) :
  SinkNode{max_channels},
  output_channels{output_channels_}
{
}

void AudioMixer::write_out(float* output_buffer,
                           unsigned long frames_per_buffer,
                           const PaStreamCallbackTimeInfo& time_info)
{
  std::span<float> out{output_buffer, frames_per_buffer * output_channels};
  std::fill(std::begin(out), std::end(out), 0.0);
  std::array<float, 1024> buf{};
  assert(buf.size() > frames_per_buffer);
  const auto n = static_cast<float>(source_count());
  std::span<float> span(buf.data(), frames_per_buffer * output_channels);
  visit_sources([&](SourceNode& sn)
                {
                  sn.put_data(span, time_info, output_channels);
                  std::transform(std::begin(out), std::end(out),
                                 std::begin(buf), std::begin(out),
                                 [n](const float& sig_o, const float& sig_c)
                                 { return sig_o + (sig_c / n); });
                });
}

}  // namespace mt::snd
