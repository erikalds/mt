#ifndef AUDIO_DATA_PRESENTER_H
#define AUDIO_DATA_PRESENTER_H

#include <SFML/System/Time.hpp>
#include <cstdint>

namespace mt {

  template<typename T> class sample_data_iterator;


  class AudioDataPresenter
  {
  public:
    virtual ~AudioDataPresenter() = 0;

    virtual void present_details(unsigned int /*sample_rate*/,
                                 const sf::Time& /*duration*/) {}
    virtual void present_channel(sample_data_iterator<const std::int16_t> begin,
                                 sample_data_iterator<const std::int16_t> end) = 0;
  };


  inline AudioDataPresenter::~AudioDataPresenter() = default;

}  // namespace mt

#endif /* AUDIO_DATA_PRESENTER_H */
