#ifndef SOURCENODE_H
#define SOURCENODE_H

#include "sndmix/node.h"
#include <span>

struct PaStreamCallbackTimeInfo;

namespace mt::snd {

class SourceNode : public Node
{
public:
  SourceNode(const SourceNode&) = delete;
  SourceNode& operator=(const SourceNode&) = delete;
  SourceNode(SourceNode&&) = default;
  SourceNode& operator=(SourceNode&&) = default;
  ~SourceNode() override = 0;

  virtual void put_data(std::span<float> output_buffer,
                        const PaStreamCallbackTimeInfo& time_info,
                        std::size_t num_output_channels) = 0;

protected:
  SourceNode() = default;
};

}  // namespace mt::snd

inline mt::snd::SourceNode::~SourceNode() = default;

#endif /* SOURCENODE_H */
