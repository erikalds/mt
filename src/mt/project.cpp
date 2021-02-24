#include "project.h"

#include "instrument.h"

Project::Project(std::string title_) :
  title{std::move(title_)}
{
  // TODO: Only for test
  instruments.emplace_back(Instrument{"01.wav"});
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
