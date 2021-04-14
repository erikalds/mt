#ifndef PATTERN_QUEUE_H
#define PATTERN_QUEUE_H

#include <deque>
#include <iterator>
#include <memory>
#include <vector>

namespace YAML { class Node; }

namespace mt {

  class Pattern;

  class PatternQueue
  {
  public:
    PatternQueue();
    PatternQueue(const PatternQueue&) = delete;
    PatternQueue& operator=(const PatternQueue&) = delete;
    PatternQueue(PatternQueue&&) noexcept;
    PatternQueue& operator=(PatternQueue&&) noexcept;
    ~PatternQueue();

    class const_iterator;

    /// Appends pattern pointed to by iter at the position after iter.
    void append_duplicate_pattern(const_iterator iter);

    /// Removes pattern from queue pointed to by iter.
    ///
    /// If iter points to the only pattern in queue, the pattern is not removed.
    void remove_pattern_from_queue(const_iterator iter);

    /// Set next pattern idx at pattern pointed to by iter.
    ///
    /// If there is no such pattern, a pattern is created.
    void increment_pattern_at(const_iterator iter);

    /// Set previous pattern idx at pattern pointed to by iter.
    ///
    /// If pattern idx is already at lowest (0), it stays at 0.
    void decrement_pattern_at(const_iterator iter);

    static std::unique_ptr<PatternQueue> load_from_yaml(const YAML::Node& node);
    [[nodiscard]] YAML::Node get_as_yaml() const;

  private:
    void add_pattern();
    [[nodiscard]] std::size_t get_pattern_idx(std::size_t qidx) const;
    [[nodiscard]] std::size_t get_pattern_idx_checked(std::size_t qidx) const;

  public:
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] Pattern& at(std::size_t idx);
    [[nodiscard]] const Pattern& at(std::size_t idx) const;
    [[nodiscard]] Pattern& operator[](std::size_t idx);
    [[nodiscard]] const Pattern& operator[](std::size_t idx) const;

    template<typename T>
    class iterator_base
    {
    public:
      using iterator_category = std::random_access_iterator_tag;
      using value_type = Pattern;
      using difference_type = ssize_t;
      using pointer = value_type*;
      using const_pointer = const value_type*;
      using reference = value_type&;
      using const_reference = const value_type&;

      bool operator==(const iterator_base<T>& other) const;
      bool operator<(const iterator_base<T>& other) const;

      T& operator+=(difference_type x);
      T& operator++() { return (*this) += 1; }
      T operator++(int);

      T& operator-=(difference_type x);
      T& operator--() { return (*this) -= 1; }
      T operator--(int);

      difference_type operator-(const iterator_base<T>& rhs) const;

      const_reference operator[](difference_type offset) const;
      const_reference operator*() const { return operator[](0); }
      const_pointer operator->() const { return &operator[](0); }

    protected:
      [[nodiscard]] std::size_t get_offset_pos(difference_type offset) const;
    };

    class iterator : public iterator_base<iterator>
    {
    public:
      iterator(std::vector<std::unique_ptr<Pattern>>& p,
               std::deque<std::size_t>& q,
               std::size_t pos_);

      reference operator[](difference_type offset);
      reference operator*() { return operator[](0); }
      pointer operator->() { return &operator[](0); }

    private:
      std::vector<std::unique_ptr<Pattern>>& patterns;
      std::deque<std::size_t>& queue;
      std::size_t pos = 0;

      friend class PatternQueue::const_iterator;
      friend class PatternQueue::iterator_base<iterator>;
    };

    class const_iterator : public iterator_base<const_iterator>
    {
    public:
      const_iterator(const std::vector<std::unique_ptr<Pattern>>& p,
                     const std::deque<std::size_t>& q,
                     std::size_t pos_);
      const_iterator(const iterator& iter); // NOLINT: allow implicit conversion

    private:
      const std::vector<std::unique_ptr<Pattern>>& patterns;
      const std::deque<std::size_t>& queue;
      std::size_t pos = 0;

      friend class PatternQueue;
      friend class PatternQueue::iterator_base<const_iterator>;
    };

    [[nodiscard]] iterator begin();
    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] iterator end();
    [[nodiscard]] const_iterator end() const;

  private:
    std::vector<std::unique_ptr<Pattern>> patterns;
    std::deque<std::size_t> queue;
  };



  // iterator_base -- out-of-class operators

  template<typename T>
  inline bool operator!=(const PatternQueue::iterator_base<T>& x,
                         const PatternQueue::iterator_base<T>& y)
  {
    return !(x == y);
  }

  template<typename T>
  inline bool operator<=(const PatternQueue::iterator_base<T>& x,
                         const PatternQueue::iterator_base<T>& y)
  {
    return x < y || x == y;
  }

  template<typename T>
  inline bool operator>(const PatternQueue::iterator_base<T>& x,
                        const PatternQueue::iterator_base<T>& y)
  {
    return !(x <= y);
  }

  template<typename T>
  inline bool operator>=(const PatternQueue::iterator_base<T>& x,
                         const PatternQueue::iterator_base<T>& y)
  {
    return !(x < y);
  }

  template<typename T>
  inline T
  operator+(const PatternQueue::iterator_base<T>& x,
            typename PatternQueue::iterator_base<T>::difference_type i)
  {
    T copy = *static_cast<const T*>(&x);
    return copy += i;
  }

  template<typename T>
  inline T
  operator+(typename PatternQueue::iterator_base<T>::difference_type i,
            const PatternQueue::iterator_base<T>& x)
  {
    return x + i;
  }

  template<typename T>
  inline T
  operator-(const PatternQueue::iterator_base<T>& x,
            typename PatternQueue::iterator_base<T>::difference_type i)
  {
    T copy = *static_cast<const T*>(&x);
    return copy -= i;
  }

  // TODO: will not this be ill-formed???
  // inline PatternQueue::iterator
  // operator-(typename PatternQueue::iterator::difference_type i,
  //           const PatternQueue::iterator& x)
  // {
  //   return x - i;
  // }


  // iterator_base -- mem.fun. implementations

  template<typename T>
  inline bool
  PatternQueue::iterator_base<T>::operator==(const iterator_base<T>& other) const
  {
    return (&static_cast<const T*>(&other)->queue
            == &static_cast<const T*>(this)->queue)
      && (static_cast<const T*>(&other)->pos
          == static_cast<const T*>(this)->pos);
  }

  template<typename T>
  inline bool
  PatternQueue::iterator_base<T>::operator<(const iterator_base<T>& other) const
  {
    return (static_cast<const T*>(this)->pos
            < static_cast<const T*>(&other)->pos);
  }

  template<typename T>
  inline T& PatternQueue::iterator_base<T>::operator+=(difference_type x)
  {
    if (x < 0)
    {
      return operator-=(std::abs(x));
    }

    static_cast<T*>(this)->pos += static_cast<std::size_t>(x);
    return *static_cast<T*>(this);
  }

  template<typename T>
  inline T PatternQueue::iterator_base<T>::operator++(int)
  {
    auto copy = *static_cast<T*>(this);
    ++(*this);
    return copy;
  }

  template<typename T>
  inline T& PatternQueue::iterator_base<T>::operator-=(difference_type x)
  {
    if (x < 0)
    {
      return operator+=(std::abs(x));
    }

    auto ux = static_cast<std::size_t>(x);
    if (static_cast<T*>(this)->pos < ux)
    {
      throw std::logic_error{"Cannot iterate before the beginning of queue"};
    }
    static_cast<T*>(this)->pos -= ux;
    return *static_cast<T*>(this);
  }

  template<typename T>
  inline T PatternQueue::iterator_base<T>::operator--(int)
  {
    auto copy = *static_cast<T*>(this);
    --(*this);
    return copy;
  }

  template<typename T>
  inline typename PatternQueue::iterator_base<T>::difference_type
  PatternQueue::iterator_base<T>::operator-(const iterator_base<T>& rhs) const
  {
    // TODO: Ignores when pos is greater than max difference_type
    return static_cast<difference_type>(static_cast<const T*>(this)->pos)
      - static_cast<difference_type>(static_cast<const T*>(&rhs)->pos);
  }

  template<typename T>
  inline typename PatternQueue::iterator_base<T>::const_reference
  PatternQueue::iterator_base<T>::operator[](difference_type offset) const
  {
    const auto qidx = get_offset_pos(offset);
    const auto& pidx = static_cast<const T*>(this)->queue.at(qidx);
    return *static_cast<const T*>(this)->patterns.at(pidx);
  }

  template<typename T>
  [[nodiscard]] inline std::size_t
  PatternQueue::iterator_base<T>::get_offset_pos(difference_type offset) const
  {
    if (offset < 0)
    {
      auto uoffset = static_cast<std::size_t>(std::abs(offset));
      if (uoffset > static_cast<const T*>(this)->pos)
      {
        throw std::logic_error{"Cannot iterate before the beginning of queue"};
      }
      return static_cast<const T*>(this)->pos - uoffset;
    }
    return static_cast<const T*>(this)->pos + static_cast<std::size_t>(offset);
  }
}  // namespace mt

#endif /* PATTERN_QUEUE_H */
