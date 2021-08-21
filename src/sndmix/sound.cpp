#include "sndmix/sound.h"
#include "sndmix/sample_data_iterator.h"
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
                  std::uint32_t /*output_sample_rate*/,
                  std::size_t offset)
  {
    const auto cc = buf->get_channel_count();
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
      liter += offset / cc;
      sample_data_iterator<const float> riter{buf->get_float_samples(),
                                              buf->get_sample_count() * cc,
                                              cc, 1};
      riter += offset / cc;
      sample_data_iterator<const float> send{};
      for (; liter != send && riter != send && oiter != oend;
           ++liter, ++riter, ++oiter)
      {
        *oiter = (*liter + *riter) / static_cast<float>(cc);
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
        siter += offset / cc;
        for (; siter != send && oiter != oend; ++siter, ++oiter)
        {
          *oiter = *siter;
        }
      }
    }
    auto next_offset = offset + (cc * output_buffer.size()) / output_channels;
    if (next_offset < buf->get_sample_count() * cc)
    {
      return next_offset;
    }
    return {};
  }

}  // namespace mt::snd
