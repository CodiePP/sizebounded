
#include "sizebounded.hpp"

template <typename T, int sz>
T sizeboundediter<T,sz>::operator* () const
{
  return _under->get(_pos);
}

template <typename T, int sz>
T sizebounded<T,sz>::get(int i) const
{
  return _buffer[i];
}

template <typename T, int sz>
sizebounded<T,sz>::sizebounded()
{
  _buffer = new T[sz];
}

template <typename T, int sz>
const T& sizebounded<T,sz>::operator[](std::size_t i) const
{
  if (i >= 0 && i < sz) {
    return _buffer[i];
  } else {
#if __cpp_exceptions
    throw sizebounded_read_exception();
#else
    return *_dummy;
#endif
  }
}

template <typename T, int sz>
T& sizebounded<T,sz>::operator[](std::size_t i)
{
  if (i >= 0 && i < sz) {
    return *(_buffer+i);
  } else {
#if __cpp_exceptions
    throw sizebounded_write_exception();
#else
    return *_dummy;
#endif
  }
}

