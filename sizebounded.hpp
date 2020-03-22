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

template <typename T, std::size_t sz>
class sizebounded;

template <typename T, std::size_t sz>
class sizeboundediter {
  public:
    sizeboundediter(const sizebounded<T,sz> *under, int pos)
      : _pos(pos), _under(under)
    {
    }

    bool operator!=(const sizeboundediter<T,sz> & other) {
      return _pos != other._pos;
    }

    const T& operator*() const {
      return _under->get(_pos);
    }

    const sizeboundediter<T,sz>& operator++() {
      ++_pos;
      return *this;
    }

  private:
    int _pos;
    const sizebounded<T, sz> * _under;
};

template <typename T, std::size_t sz>
class sizebounded {
  public:
  sizebounded();
  ~sizebounded();

  sizebounded(sizebounded const &);
  sizebounded& operator=(sizebounded const &);

  T& operator[](std::size_t i);
  const T& operator[](std::size_t i) const;

  const T& get(std::size_t i) const;
  sizeboundediter<T,sz> begin() const {
        return sizeboundediter<T,sz>(this, 0);
  }
 
  sizeboundediter<T,sz> end() const {
        return sizeboundediter<T,sz>(this, sz);
  }

  std::size_t size() const { return sz; }

  std::string toString() const;
  std::vector<T> toVector() const;

  void map(std::function<void(const int, const T)>) const;
  void zip(std::function<void(const T, const T)>, sizebounded<T,sz> const &) const;
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

template <typename T1, typename T2, std::size_t sz1, std::size_t sz2>
bool memcopy(sizebounded<T2,sz2> &target, const sizebounded<T1,sz1> &source, std::size_t nsrc);
