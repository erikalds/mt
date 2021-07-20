#ifndef SNDMIX_NODE_H
#define SNDMIX_NODE_H

namespace mt::snd {

class Node
{
public:
  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;
  Node(Node&&) = default;
  Node& operator=(Node&&) = default;
  virtual ~Node() = 0;

protected:
  Node() = default;
};

}  // namespace mt::snd

inline mt::snd::Node::~Node() = default;

#endif /* SNDMIX_NODE_H */
