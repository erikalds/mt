#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "sndmix/node.h"
#include <span>
#include <vector>

struct PaStreamCallbackTimeInfo;

namespace mt::snd {

class SinkNode;

class SourceNode : public Node
{
public:
  SourceNode(const SourceNode&) = delete;
  SourceNode& operator=(const SourceNode&) = delete;
  SourceNode(SourceNode&&) noexcept;
  SourceNode& operator=(SourceNode&&) noexcept;
  ~SourceNode() override = 0;

  virtual void put_data(std::span<float> output_buffer,
                        const PaStreamCallbackTimeInfo& time_info,
                        std::size_t num_output_channels) = 0;

  void sink_added(SinkNode& sn); // should only be called by SinkNode
  void sink_removed(SinkNode& sn); // should only be called by SinkNode

protected:
  SourceNode() = default;

private:
  void remove_from_sinks();
  void move_sinks_from(SourceNode& src);

  template<typename FunT>
  void for_each_sink(FunT fun);

  std::vector<SinkNode*> sinks = {};
};

}  // namespace mt::snd

#endif /* SOURCENODE_H */
