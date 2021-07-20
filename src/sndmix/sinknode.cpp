#include "sinknode.h"

#include <fmt/format.h>
#include <stdexcept>

namespace mt::snd {

void SinkNode::add_source(SourceNode& sn)
{
  for (auto iter = std::begin(sources); iter != std::end(sources); ++iter)
  {
    if (*iter == nullptr)
    {
      *iter = &sn;
      ++N;
      return;
    }
  }
  throw std::runtime_error(fmt::format("Max sources for SinkNode exceeded ({})",
                                       sources.size()));
}

SinkNode::SinkNode(std::size_t max_source_count) :
  sources(max_source_count, nullptr)
{
}

void SinkNode::visit_sources(std::function<void(SourceNode&)> fun)
{
  for (auto* sn : sources)
  {
    if (sn != nullptr)
    {
      fun(*sn);
    }
  }
}

void SinkNode::visit_sources(std::function<void(const SourceNode&)> fun) const
{
  for (const auto* sn : sources)
  {
    if (sn != nullptr)
    {
      fun(*sn);
    }
  }
}

[[nodiscard]] std::size_t SinkNode::source_count() const
{
  return N;
}

}  // namespace mt::snd
