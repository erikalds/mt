#include "pattern_queue.h"

#include "mtlib/pattern.h"
#include <yaml-cpp/yaml.h>
#include <stdexcept>

namespace mt {

  PatternQueue::PatternQueue() :
    queue{0}
  {
    add_pattern();
  }

  PatternQueue::PatternQueue(PatternQueue&&) noexcept = default;
  PatternQueue& PatternQueue::operator=(PatternQueue&&) noexcept = default;
  PatternQueue::~PatternQueue() = default;


  void PatternQueue::append_duplicate_pattern(const_iterator iter)
  {
    const auto pos = static_cast<std::deque<std::size_t>::difference_type>(iter.pos);
    queue.insert(queue.begin() + pos + 1, *(queue.begin() + pos));
  }

  void PatternQueue::remove_pattern_from_queue(const_iterator iter)
  {
    if (queue.size() == 1)
    {
      return;
    }

    const auto pos = static_cast<std::deque<std::size_t>::difference_type>(iter.pos);
    queue.erase(queue.begin() + pos);
  }

  void PatternQueue::increment_pattern_at(const_iterator iter)
  {
    if (++queue[iter.pos] >= patterns.size())
    {
      add_pattern();
    }
  }

  void PatternQueue::decrement_pattern_at(const_iterator iter)
  {
    if (queue[iter.pos] > 0)
    {
      --queue[iter.pos];
    }
  }

  std::unique_ptr<PatternQueue> PatternQueue::load_from_yaml(const YAML::Node& node)
  {
    auto pq = std::make_unique<PatternQueue>();
    pq->patterns.clear();
    for (const auto& pn : node["patterns"])
    {
      pq->patterns.push_back(Pattern::load_from_yaml(pn));
    }
    pq->queue.clear();
    for (const auto& qelem : node["queue"])
    {
      pq->queue.push_back(qelem.as<std::size_t>());
    }
    return pq;
  }

  YAML::Node PatternQueue::get_as_yaml() const
  {
    YAML::Node node{};
    for (const auto& p : patterns)
    {
      node["patterns"].push_back(p->get_as_yaml());
    }
    for (const auto& qidx : queue)
    {
      node["queue"].push_back(qidx);
    }
    return node;
  }

  void PatternQueue::add_pattern()
  {
    patterns.push_back(std::make_unique<Pattern>(fmt::format("{:02x}", patterns.size()),
                                                 0x40, 8));
  }

  std::size_t PatternQueue::get_pattern_idx(std::size_t qidx) const
  {
    return queue[qidx];
  }

  std::size_t PatternQueue::get_pattern_idx_checked(std::size_t qidx) const
  {
    if (qidx >= queue.size())
    {
      throw std::out_of_range(fmt::format("Index too high: {}, size was {}.",
                                          qidx, queue.size()));
    }
    auto pidx = queue[qidx];
    if (pidx >= patterns.size())
    {
      throw std::out_of_range(fmt::format("Queue index too high: {} [{}], size was {}.",
                                          pidx, qidx, patterns.size()));
    }
    return pidx;
  }

  std::size_t PatternQueue::size() const
  {
    return queue.size();
  }

  Pattern& PatternQueue::at(std::size_t idx)
  {
    return *patterns[get_pattern_idx_checked(idx)];
  }

  const Pattern& PatternQueue::at(std::size_t idx) const
  {
    return *patterns[get_pattern_idx_checked(idx)];
  }

  Pattern& PatternQueue::operator[](std::size_t idx)
  {
    return *patterns[get_pattern_idx(idx)];
  }

  const Pattern& PatternQueue::operator[](std::size_t idx) const
  {
    return *patterns[get_pattern_idx(idx)];
  }



  PatternQueue::iterator::iterator(std::vector<std::unique_ptr<Pattern>>& p,
                                   std::deque<std::size_t>& q,
                                   std::size_t pos_) :
    patterns{p},
    queue{q},
    pos{pos_}
  {
  }

  PatternQueue::iterator::reference
  PatternQueue::iterator::operator[](difference_type offset)
  {
    return *patterns.at(queue.at(get_offset_pos(offset)));
  }



  PatternQueue::const_iterator::const_iterator(const std::vector<std::unique_ptr<Pattern>>& p,
                                               const std::deque<std::size_t>& q,
                                               std::size_t pos_) :
    patterns{p},
    queue{q},
    pos{pos_}
  {
  }

  PatternQueue::const_iterator::const_iterator(const iterator& iter) :
    patterns{iter.patterns},
    queue{iter.queue},
    pos{iter.pos}
  {
  }



  PatternQueue::iterator PatternQueue::begin()
  {
    return iterator(patterns, queue, 0);
  }

  PatternQueue::const_iterator PatternQueue::begin() const
  {
    return const_iterator(patterns, queue, 0);
  }

  PatternQueue::iterator PatternQueue::end()
  {
    return iterator(patterns, queue, queue.size());
  }

  PatternQueue::const_iterator PatternQueue::end() const
  {
    return const_iterator(patterns, queue, queue.size());
  }

}  // namespace mt
