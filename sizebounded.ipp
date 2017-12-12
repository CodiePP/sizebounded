
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
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "CTOR sizebounded" << std::endl; }
#endif
}

template <typename T, int sz>
sizebounded<T,sz>::~sizebounded()
{
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "DTOR sizebounded" << std::endl; }
#endif
  if (_buffer) { delete[] _buffer; }
}

template <typename T, int sz>
sizebounded<T,sz>::sizebounded(sizebounded<T,sz> const & sb2)
{
  _buffer = new T[sz];
  memcpy(_buffer, sb2._buffer, sz);
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "sizebounded(sizebounded const &)" << std::endl; }
#endif
}

template <typename T, int sz>
sizebounded<T,sz>& sizebounded<T,sz>::operator=(sizebounded<T,sz> const & sb2)
{
  _buffer = new T[sz];
  memcpy(_buffer, sb2._buffer, sz);
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "operator=(sizebounded const &)" << std::endl; }
#endif
  return *this;
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

template <typename T, int sz>
std::string sizebounded<T,sz>::toString() const
{
  // this might return quite some nonsense for other T
  // we need some partial specialisation here
  return std::string((const char*)_buffer, sz); 
}

template <typename T, int sz>
std::vector<T> sizebounded<T,sz>::toVector() const
{
  std::vector<T> v(sz);  // allocate to size
  for (int i=0; i<sz; i++) {
    v[i] = _buffer[i]; }
  return v;
}

