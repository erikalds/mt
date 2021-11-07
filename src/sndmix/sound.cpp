#include "sndmix/sound.h"
#include "sndmix/interpolating_sample_data_iterator.h"
#include "sndmix/soundbuffer.h"

namespace mt::snd {

  Sound::Sound(const SoundBuffer& buf_, double pitch_) :
    buf{&buf_},
    pitch{pitch_}
  {
  }

  std::optional<std::size_t>
  Sound::write_to(std::span<float> output_buffer,
                  std::size_t output_channels,
                  std::uint32_t output_sample_rate,
                  std::size_t offset)
  {
    const auto cc = buf->get_channel_count();
    const auto sample_rate_ratio = static_cast<double>(buf->get_sample_rate()) / output_sample_rate;
    // only special case we handle: downmix stereo to mono
    if (output_channels < cc && output_channels == 1 && cc == 2)
    {
      sample_data_iterator<float> oiter{output_buffer.data(),
                                        output_buffer.size(),
                                        output_channels};
      sample_data_iterator<float> oend{};
      sample_data_iterator<const float> liter{buf->get_float_samples(),
                                              buf->get_sample_count() * cc,
                                              cc, 0};
      liter += static_cast<std::size_t>(sample_rate_ratio * static_cast<double>(offset) / cc);
      sample_data_iterator<const float> riter{buf->get_float_samples(),
                                              buf->get_sample_count() * cc,
                                              cc, 1};
      riter += static_cast<std::size_t>(sample_rate_ratio * static_cast<double>(offset) / cc);
      sample_data_iterator<const float> send{};
      interpolating_sample_data_iterator<const float>
        liiter{liter, send, buf->get_sample_rate(), output_sample_rate};
      interpolating_sample_data_iterator<const float>
        riiter{riter, send, buf->get_sample_rate(), output_sample_rate};
      interpolating_sample_data_iterator<const float> isend{};
      for (; liiter != isend && riiter != isend && oiter != oend;
           ++liiter, ++riiter, ++oiter)
      {
        *oiter = (*liiter + *riiter) / static_cast<float>(cc);
      }
    }
    else
    {
      for (std::size_t i = 0; i < output_channels; ++i)
      {
        sample_data_iterator<float> oiter{output_buffer.data(),
                                          output_buffer.size(),
                                          output_channels, i};
        sample_data_iterator<float> oend{};
        sample_data_iterator<const float> siter{buf->get_float_samples(),
                                                buf->get_sample_count() * cc,
                                                cc, i % cc};
        sample_data_iterator<const float> send{};
        siter += static_cast<std::size_t>(sample_rate_ratio * static_cast<double>(offset) / cc);
        interpolating_sample_data_iterator<const float>
          isiter{siter, send, buf->get_sample_rate(), output_sample_rate};
        interpolating_sample_data_iterator<const float> isend{};
        for (; isiter != isend && oiter != oend; ++isiter, ++oiter)
        {
          *oiter = *isiter;
        }
      }
    }
    auto next_offset = offset + (cc * output_buffer.size()) / output_channels;
    if (static_cast<double>(next_offset) < buf->get_sample_count() * cc / sample_rate_ratio)
    {
      return next_offset;
    }
    return {};
  }

}  // namespace mt::snd
