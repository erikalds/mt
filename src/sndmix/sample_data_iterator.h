#ifndef SAMPLE_DATA_ITERATOR_H
#define SAMPLE_DATA_ITERATOR_H

#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>

namespace mt::snd {

  template<typename T>
  class sample_data_iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::size_t;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;

    sample_data_iterator() = default;
    sample_data_iterator(pointer data_, difference_type count_, difference_type channels_=1,
                         difference_type interleave_offset_=0) :
      data{data_}, i{interleave_offset_}, count{count_}, channels{channels_}
    {
      assert(interleave_offset_ < channels);
    }

    bool operator==(const sample_data_iterator& other) const;
    bool operator<(const sample_data_iterator& other) const;

    sample_data_iterator& operator+=(difference_type x) { i += channels * x; return *this; }
    sample_data_iterator& operator-=(difference_type x);
    reference operator[](difference_type offset) { return data[i + channels * offset]; }
    const_reference operator[](difference_type offset) const { return data[i + channels * offset]; }
    reference operator*() { return operator[](0); }
    const_reference operator*() const { return operator[](0); }
    difference_type operator-(const sample_data_iterator& rhs) const;

  private:
    pointer data = nullptr;
    difference_type i = 0;
    difference_type count = 0;
    difference_type channels = 0;
  };

  template<typename T>
  inline bool operator!=(const sample_data_iterator<T>& x,
                         const sample_data_iterator<T>& y)
  {
    return !(x == y);
  }

  template<typename T>
  inline bool operator<=(const sample_data_iterator<T>& x,
                         const sample_data_iterator<T>& y)
  {
    return x < y || x == y;
  }

  template<typename T>
  inline bool operator>(const sample_data_iterator<T>& x,
                        const sample_data_iterator<T>& y)
  {
    return !(x <= y);
  }
  template<typename T>
  inline bool operator>=(const sample_data_iterator<T>& x,
                         const sample_data_iterator<T>& y)
  {
    return !(x < y);
  }

  template<typename T>
  inline sample_data_iterator<T>& operator++(sample_data_iterator<T>& x)
  {
    return x += 1;
  }

  template<typename T>
  inline sample_data_iterator<T> operator++(sample_data_iterator<T>& x, int)
  {
    auto copy = x;
    ++x;
    return copy;
  }

  template<typename T>
  inline sample_data_iterator<T>& operator--(sample_data_iterator<T>& x)
  {
    return x -= 1;
  }

  template<typename T>
  inline sample_data_iterator<T> operator--(sample_data_iterator<T>& x, int)
  {
    auto copy = x;
    --x;
    return copy;
  }

  template<typename T>
  inline sample_data_iterator<T> operator+(const sample_data_iterator<T>& x,
                                           typename sample_data_iterator<T>::difference_type i)
  {
    auto copy = x;
    return copy += i;
  }

  template<typename T>
  inline sample_data_iterator<T> operator+(typename sample_data_iterator<T>::difference_type i,
                                           const sample_data_iterator<T>& x)
  {
    return x + i;
  }

  template<typename T>
  inline sample_data_iterator<T> operator-(const sample_data_iterator<T>& x,
                                           typename sample_data_iterator<T>::difference_type i)
  {
    auto copy = x;
    return copy -= i;
  }

  // TODO: will not this be ill-formed???
  // template<typename T>
  // inline sample_data_iterator<T> operator-(difference_type i,
  //                                          const sample_data_iterator<T>& x)
  // {
  //   return x - i;
  // }



  template<typename T>
  inline bool
  sample_data_iterator<T>::operator==(const sample_data_iterator& other) const
  {
    return (this->data == nullptr && other.i >= other.count) // == end iter
      || (other.data == nullptr && this->i >= this->count) // == end iter
      || (this->data == other.data && this->i == other.i
          && this->count == other.count && this->channels == other.channels);
  }

  template<typename T>
  inline bool
  sample_data_iterator<T>::operator<(const sample_data_iterator& other) const
  {
    return this->data != nullptr
      && ((!operator==(other) && this->i < other.i)
          || (other.data == nullptr && this->i < this->count));
  }

  template<typename T>
  inline sample_data_iterator<T>& sample_data_iterator<T>::operator-=(difference_type x)
  {
    if (i < channels * x)
    {
      throw std::logic_error{"Cannot iterate before the beginning of the "
                               "underlying data pointer"};
    }

    i -= channels * x;
    return *this;
  }

  template<typename T>
  inline typename sample_data_iterator<T>::difference_type
  sample_data_iterator<T>::operator-(const sample_data_iterator& rhs) const
  {
    if (this->data == nullptr && rhs.data != nullptr)
    {
      return (rhs.count - rhs.i + rhs.channels - 1) / rhs.channels;
    }
    if (rhs.data == nullptr && this->data != nullptr)
    {
      return (this->count - this->i + this->channels - 1) / this->channels;
    }
    if (rhs.data == nullptr && this->data == nullptr)
    {
      return 0;
    }
    assert(rhs.data == this->data && rhs.channels == this->channels);
    return (std::max(rhs.i, this->i) - std::min(rhs.i, this->i)) / rhs.channels;
  }

}  // namespace mt::snd

#endif /* SAMPLE_DATA_ITERATOR_H */
