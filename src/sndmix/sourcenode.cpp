#include "sourcenode.h"

#include "sinknode.h"
#include <algorithm>

namespace mt::snd {

SourceNode::~SourceNode()
{
  remove_from_sinks();
}

SourceNode::SourceNode(SourceNode&& other) noexcept
{
  move_sinks_from(other);
}

SourceNode& SourceNode::operator=(SourceNode&& other) noexcept
{
  if (this != &other)
  {
    remove_from_sinks();
    move_sinks_from(other);
  }
  return *this;
}

void SourceNode::sink_added(SinkNode& sn)
{
  sinks.push_back(&sn);
}

void SourceNode::sink_removed(SinkNode& sn)
{
  auto iter = std::ranges::find(sinks, &sn);
  if (iter != std::end(sinks))
  {
    sinks.erase(iter);
  }
}

void SourceNode::remove_from_sinks()
{
  for_each_sink([&](SinkNode& s) { s.remove_source(*this); });
}

void SourceNode::move_sinks_from(SourceNode& src)
{
  src.for_each_sink([&](SinkNode& s)
                    {
                      s.remove_source(src);
                      s.add_source(*this);
                    });
}

template<typename FunT>
void SourceNode::for_each_sink(FunT fun)
{
  // remove_source calls sink_removed. Take a copy of sinks so that
  // iterators are not affected.
  auto sinks_copy = sinks;
  std::ranges::for_each(sinks_copy, [&](SinkNode* s) { fun(*s); });
}

}  // namespace mt::snd
