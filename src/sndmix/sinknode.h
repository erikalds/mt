#ifndef SINKNODE_H
#define SINKNODE_H

#include "sndmix/node.h"
#include <bits/c++config.h> // for std::size_t
#include <functional>

namespace mt::snd {

class SourceNode;

class SinkNode : public Node
{
public:
  SinkNode(const SinkNode&) = delete;
  SinkNode& operator=(const SinkNode&) = delete;
  SinkNode(SinkNode&&) = default;
  SinkNode& operator=(SinkNode&&) = default;
  ~SinkNode() override = 0;

  void add_source(SourceNode& sn);

protected:
  explicit SinkNode(std::size_t max_source_count);

  void visit_sources(std::function<void(SourceNode&)> fun);
  void visit_sources(std::function<void(const SourceNode&)> fun) const;
  [[nodiscard]] std::size_t source_count() const;

private:
  std::vector<SourceNode*> sources;
  std::size_t N = 0;
};

}  // namespace mt::snd


inline mt::snd::SinkNode::~SinkNode() = default;

#endif /* SINKNODE_H */
