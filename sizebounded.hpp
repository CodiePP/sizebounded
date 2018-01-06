#pragma once

#include <cstddef>
#include <exception>
#include <string>
#include <vector>
#include <functional>


#if __cpp_exceptions
class sizebounded_read_exception : public std::exception {};
class sizebounded_write_exception : public std::exception {};
#endif

template <typename T, int sz>
class sizebounded;

template <typename T, int sz>
class sizeboundediter {
  public:
    sizeboundediter(const sizebounded<T,sz> *under, int pos)
      : _pos(pos), _under(under)
    {
    }

    bool operator!=(const sizeboundediter<T,sz> & other) {
      return _pos != other._pos;
    }

    T operator*() const;

    const sizeboundediter<T,sz>& operator++() {
      ++_pos;
      return *this;
    }

  private:
    int _pos;
    const sizebounded<T, sz> * _under;
};

template <typename T, int sz>
class sizebounded {
  public:
  sizebounded();
  ~sizebounded();

  sizebounded(sizebounded const &);
  sizebounded& operator=(sizebounded const &);

  T& operator[](std::size_t i);
  const T& operator[](std::size_t i) const;

  T get(int) const;
  sizeboundediter<T,sz> begin() const {
        return sizeboundediter<T,sz>(this, 0);
  }
 
  sizeboundediter<T,sz> end() const {
        return sizeboundediter<T,sz>(this, sz);
  }

  int size() const { return sz; }

  std::string toString() const;
  std::vector<T> toVector() const;

  void map(std::function<void(const int, const T)>) const;
  void transform(std::function<T(const int, const T)>);

  const T* ptr() const { return _buffer; }

#if __cpp_exceptions
#else
  bool isValid(const T& t) const { return &t != &_dummy; }
#endif
  private:
    T *_buffer;
#if __cpp_exceptions
#else
    T *_dummy { nullptr };
#endif
};

