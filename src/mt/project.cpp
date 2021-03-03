#include "project.h"

#include "instrument.h"

Project::Project(std::string title_) :
  title{std::move(title_)}
{
  // TODO: Only for test
  instruments.emplace_back(Instrument{"01.wav"});
  instruments.emplace_back(Instrument{"02.wav"});
  instruments.emplace_back(Instrument{"c-2.wav"});
  instruments.emplace_back(Instrument{"c-3.wav"});
  instruments.emplace_back(Instrument{"c-4.wav"});
  instruments.emplace_back(Instrument{"c-5.wav"});
  instruments.emplace_back(Instrument{"c-6.wav"});
}

Project::~Project() = default;

Instrument* Project::get_instrument(std::size_t idx)
{
  if (idx < instruments.size())
  {
    return &instruments[idx];
  }

  return nullptr;
}

const Instrument* Project::get_instrument(std::size_t idx) const
{
  if (idx < instruments.size())
  {
    return &instruments[idx];
  }

  return nullptr;
}

std::size_t Project::instrument_count() const
{
  return instruments.size();
}
