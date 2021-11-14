#include "sinknode.h"

#include "sourcenode.h"

#include <fmt/format.h>
#include <stdexcept>

namespace mt::snd {

SinkNode::SinkNode(SinkNode&& other) noexcept :
  sources{std::move(other.sources)},
  N{other.N}
{
  move_sources_from(other);
}

SinkNode& SinkNode::operator=(SinkNode&& other) noexcept
{
  if (this != &other)
  {
    remove_sources();
    sources = std::move(other.sources);
    N = other.N;
    move_sources_from(other);
  }
  return *this;
}

SinkNode::~SinkNode()
{
  remove_sources();
}

void SinkNode::add_source(SourceNode& sn)
{
  auto iter = std::ranges::find(sources, nullptr);
  if (iter == std::end(sources))
  {
    throw std::runtime_error(fmt::format("Max sources for SinkNode exceeded ({})",
                                         sources.size()));
  }

  *iter = &sn;
  ++N;
  sn.sink_added(*this);
}

void SinkNode::remove_source(SourceNode& sn)
{
  auto iter = std::ranges::find(sources, &sn);
  if (iter != std::end(sources))
  {
    *iter = nullptr;
    --N;
    sn.sink_removed(*this);
  }
}

SinkNode::SinkNode(std::size_t max_source_count) :
  sources(max_source_count, nullptr)
{
}

void SinkNode::visit_sources(std::function<void(SourceNode&)> fun)
{
  for_each_source(std::move(fun));
}

void SinkNode::visit_sources(std::function<void(const SourceNode&)> fun) const
{
  for_each_source(std::move(fun));
}

[[nodiscard]] std::size_t SinkNode::source_count() const
{
  return N;
}

void SinkNode::move_sources_from(SinkNode& other)
{
  for_each_source([&](SourceNode& src)
                  {
                    src.sink_removed(other);
                    src.sink_added(*this);
                  });
}

void SinkNode::remove_sources()
{
  for_each_source([&](SourceNode& src) { src.sink_removed(*this); });
}

template<typename FunT>
void SinkNode::for_each_source(FunT fun) const
{
  for (auto* src : sources)
  {
    if (src != nullptr)
    {
      fun(*src);
    }
  }
}

}  // namespace mt::snd
