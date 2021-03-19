#ifndef SAMPLE_DATA_ITERATOR_H
#define SAMPLE_DATA_ITERATOR_H

#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>

namespace mt {

  template<typename T>
  class sample_data_iterator : public std::random_access_iterator_tag
  {
  public:
    sample_data_iterator() = default;
    sample_data_iterator(T* data_, std::size_t count_, std::size_t channels_=1,
                         std::size_t interleave_offset_=0) :
      data{data_}, i{interleave_offset_}, count{count_}, channels{channels_}
    {
      assert(interleave_offset_ < channels);
    }

    bool operator==(const sample_data_iterator& other) const;
    bool operator<(const sample_data_iterator& other) const;

    sample_data_iterator& operator+=(std::size_t x) { i += channels * x; return *this; }
    sample_data_iterator& operator-=(std::size_t x);
    T& operator[](std::size_t offset) { return data[i + channels * offset]; }
    const T& operator[](std::size_t offset) const { return data[i + channels * offset]; }
    T& operator*() { return operator[](0); }
    const T& operator*() const { return operator[](0); }

  private:
    T* data = nullptr;
    std::size_t i = 0;
    std::size_t count = 0;
    std::size_t channels = 0;
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
                                           std::size_t i)
  {
    auto copy = x;
    return copy += i;
  }

  template<typename T>
  inline sample_data_iterator<T> operator+(std::size_t i,
                                           const sample_data_iterator<T>& x)
  {
    return x + i;
  }

  template<typename T>
  inline sample_data_iterator<T> operator-(const sample_data_iterator<T>& x,
                                           std::size_t i)
  {
    auto copy = x;
    return copy -= i;
  }

  // TODO: will not this be ill-formed???
  // template<typename T>
  // inline sample_data_iterator<T> operator-(std::size_t i,
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
  inline sample_data_iterator<T>& sample_data_iterator<T>::operator-=(std::size_t x)
  {
    if (i < channels * x)
    {
      throw std::logic_error{"Cannot iterate before the beginning of the "
                               "underlying data pointer"};
    }

    i -= channels * x;
    return *this;
  }
}  // namespace mt

#endif /* SAMPLE_DATA_ITERATOR_H */
