#ifndef INTERPOLATING_SAMPLE_DATA_ITERATOR_H
#define INTERPOLATING_SAMPLE_DATA_ITERATOR_H

#include "sndmix/sample_data_iterator.h"
#include <fmt/format.h>
#include <cmath>

namespace mt::snd {

  /// Since this is an iterator that possibly (but not necessarily) generates
  /// the data it iterates, it is inherently a const iterator.
  template<typename T>
  class interpolating_sample_data_iterator
  {
  public:
    using iterator_category = typename sample_data_iterator<T>::iterator_category;
    using const_reference = const T&;
    using reference = T&;
    using difference_type = std::size_t;
    using value_type = T;

    interpolating_sample_data_iterator() = default;
    interpolating_sample_data_iterator(sample_data_iterator<T> begin_,
                                       sample_data_iterator<T> end_,
                                       std::uint32_t sample_rate,
                                       std::uint32_t output_sample_rate);

    bool operator==(const interpolating_sample_data_iterator& other) const;
    bool operator<(const interpolating_sample_data_iterator& other) const;
    interpolating_sample_data_iterator& operator+=(difference_type x)
    { pos += x; return *this; }
    interpolating_sample_data_iterator& operator-=(difference_type x);

    difference_type operator-(const interpolating_sample_data_iterator& other) const;

    value_type operator*() const
    { return static_cast<value_type>(lower_weight() * static_cast<double>(*(begin + lower_idx()))
                                     + upper_weight() * static_cast<double>(*(begin + upper_idx()))); }

    value_type operator[](difference_type offset) const { return *(*this + offset); }

  private:
    [[nodiscard]] difference_type lower_idx() const
    { return static_cast<difference_type>(std::floor(interpolated_pos())); }
    [[nodiscard]] double lower_weight() const { return 1.0 - weight(); }
    [[nodiscard]] double upper_weight() const { return weight(); }
    [[nodiscard]] double weight() const
    { return interpolated_pos() - std::floor(interpolated_pos()); }
    [[nodiscard]] difference_type upper_idx() const
    { return static_cast<difference_type>(std::ceil(interpolated_pos())); }
    [[nodiscard]] double interpolated_pos() const { return static_cast<double>(pos) * interp; }
    [[nodiscard]] bool is_end_iter() const { return begin == end; }

    sample_data_iterator<T> begin = {};
    std::size_t pos = 0;
    sample_data_iterator<T> end = {};
    double interp = 1.0;
  };


  template<typename T>
  inline bool operator!=(const interpolating_sample_data_iterator<T>& x,
                         const interpolating_sample_data_iterator<T>& y)
  {
    return !(x == y);
  }

  template<typename T>
  inline bool operator<=(const interpolating_sample_data_iterator<T>& x,
                         const interpolating_sample_data_iterator<T>& y)
  {
    return x < y || x == y;
  }

  template<typename T>
  inline bool operator>(const interpolating_sample_data_iterator<T>& x,
                        const interpolating_sample_data_iterator<T>& y)
  {
    return !(x <= y);
  }

  template<typename T>
  inline bool operator>=(const interpolating_sample_data_iterator<T>& x,
                         const interpolating_sample_data_iterator<T>& y)
  {
    return !(x < y);
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>&
  operator++(interpolating_sample_data_iterator<T>& x)
  {
    return x += 1;
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>
  operator++(interpolating_sample_data_iterator<T>& x, int)
  {
    auto copy = x;
    ++x;
    return copy;
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>&
  operator--(interpolating_sample_data_iterator<T>& x)
  {
    return x -= 1;
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>
  operator--(interpolating_sample_data_iterator<T>& x, int)
  {
    auto copy = x;
    --x;
    return copy;
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>
  operator+(const interpolating_sample_data_iterator<T>& x,
            typename interpolating_sample_data_iterator<T>::difference_type i)
  {
    auto copy = x;
    return copy += i;
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>
  operator+(typename interpolating_sample_data_iterator<T>::difference_type i,
            const interpolating_sample_data_iterator<T>& x)
  {
    auto copy = x;
    return copy += i;
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>
  operator-(const interpolating_sample_data_iterator<T>& x,
            typename interpolating_sample_data_iterator<T>::difference_type i)
  {
    auto copy = x;
    return copy -= i;
  }



  template<typename T>
  interpolating_sample_data_iterator<T>::
  interpolating_sample_data_iterator(sample_data_iterator<T> begin_,
                                     sample_data_iterator<T> end_,
                                     std::uint32_t sample_rate,
                                     std::uint32_t output_sample_rate) :
    begin{begin_},
    end{end_},
    interp{static_cast<double>(sample_rate) / output_sample_rate}
  {
  }

  template<typename T>
  bool interpolating_sample_data_iterator<T>::
  operator==(const interpolating_sample_data_iterator& other) const
  {
    if (is_end_iter() && !other.is_end_iter())
    {
      return other.upper_idx() >= std::distance(other.begin, other.end);
    }
    if (!is_end_iter() && other.is_end_iter())
    {
      return upper_idx() >= std::distance(begin, end);
    }
    if (is_end_iter() && other.is_end_iter())
    {
      return true;
    }
    return begin == other.begin && interpolated_pos() == other.interpolated_pos();
  }

  template<typename T>
  bool interpolating_sample_data_iterator<T>::
  operator<(const interpolating_sample_data_iterator& other) const
  {
    if (is_end_iter() && !other.is_end_iter())
    {
      return false;
    }
    if (!is_end_iter() && other.is_end_iter())
    {
      return upper_idx() < std::distance(begin, end);
    }
    if (is_end_iter() && other.is_end_iter())
    {
      return false;
    }

    return begin == other.begin && interpolated_pos() < other.interpolated_pos();
  }

  template<typename T>
  inline interpolating_sample_data_iterator<T>&
  interpolating_sample_data_iterator<T>::operator-=(difference_type x)
  {
    if (x > pos)
    {
      throw std::logic_error{"Cannot iterate before the beginning of the "
                             "underlying iterator"};
    }
    pos -= x;
    return *this;
  }

  template<typename T>
  typename interpolating_sample_data_iterator<T>::difference_type
  interpolating_sample_data_iterator<T>::
  operator-(const interpolating_sample_data_iterator& other) const
  {
    if (is_end_iter() && !other.is_end_iter())
    {
      return other - *this;
    }
    if (!is_end_iter() && other.is_end_iter())
    {
      return static_cast<difference_type>(static_cast<double>(std::distance(begin, end)) / interp) - pos - 1;
    }
    if (is_end_iter() && other.is_end_iter())
    {
      return 0;
    }

    assert(begin == other.begin);
    return std::max(pos, other.pos) - std::min(pos, other.pos);
  }

} // namespace mt::snd

#endif /* INTERPOLATING_SAMPLE_DATA_ITERATOR_H */
