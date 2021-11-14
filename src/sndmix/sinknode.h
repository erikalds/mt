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
  SinkNode(SinkNode&&) noexcept;
  SinkNode& operator=(SinkNode&&) noexcept;
  ~SinkNode() override = 0;

  void add_source(SourceNode& sn);
  void remove_source(SourceNode& sn);

protected:
  explicit SinkNode(std::size_t max_source_count);

  void visit_sources(std::function<void(SourceNode&)> fun);
  void visit_sources(std::function<void(const SourceNode&)> fun) const;
  [[nodiscard]] std::size_t source_count() const;

private:
  void move_sources_from(SinkNode& other);
  void remove_sources();

  template<typename FunT>
  void for_each_source(FunT fun) const;

  std::vector<SourceNode*> sources;
  std::size_t N = 0;
};

}  // namespace mt::snd

#endif /* SINKNODE_H */
