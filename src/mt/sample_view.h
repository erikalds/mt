#ifndef SAMPLE_VIEW_H
#define SAMPLE_VIEW_H

#include "mtlib/audio_data_presenter.h"

#include <list>
#include <string>
#include <vector>


class SampleView : public mt::AudioDataPresenter
{
public:
  SampleView();

  void paint();

  void clear();
  void present_details(unsigned int sample_rate, const sf::Time& duration) override;
  void present_channel(mt::sample_data_iterator<const std::int16_t> begin,
                       mt::sample_data_iterator<const std::int16_t> end) override;

private:
  std::list<std::vector<float>> channels;
  std::string overlay;
};

#endif /* SAMPLE_VIEW_H */
