
#include "sizebounded/sizebounded.hpp"
#include <cstring>

template <typename T, std::size_t sz>
const T& sizebounded<T,sz>::get(std::size_t i) const
{
  return _buffer[i];
}

template <typename T, std::size_t sz>
sizebounded<T,sz>::sizebounded()
{
  _buffer = new T[sz];
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "CTOR sizebounded" << std::endl; }
#endif
}

template <typename T, std::size_t sz>
sizebounded<T,sz>::~sizebounded()
{
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "DTOR sizebounded" << std::endl; }
#endif
  if (_buffer) { delete[] _buffer; }
}

template <typename T, std::size_t sz>
sizebounded<T,sz>::sizebounded(sizebounded<T,sz> const & sb2)
{
  _buffer = new T[sz];
  memcopy(*this, sb2, sz);
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "sizebounded(sizebounded const &)" << std::endl; }
#endif
}

template <typename T, std::size_t sz>
sizebounded<T,sz>& sizebounded<T,sz>::operator=(sizebounded<T,sz> const & sb2)
{
  _buffer = new T[sz];
  memcopy(*this, sb2, sz);
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "operator=(sizebounded const &)" << std::endl; }
#endif
  return *this;
}

template <typename T, std::size_t sz>
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

template <typename T, std::size_t sz>
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

template <typename T, std::size_t sz>
std::string sizebounded<T,sz>::toString() const
{
  // this might return quite some nonsense for other T
  // we need some partial specialisation here
  return std::string((const char*)_buffer, sz); 
}

template <typename T, std::size_t sz>
std::vector<T> sizebounded<T,sz>::toVector() const
{
  std::vector<T> v(sz);  // allocate to size
  //for (int i=0; i<sz; i++) {
  //  v[i] = _buffer[i]; }
  map([&v](int i, const T t) { v[i] = t; });
  return v;
}

template <typename T, std::size_t sz>
void sizebounded<T,sz>::map(std::function<void(const int, const T)> f) const
{
    for (int i=0; i < sz; i++) {
        f(i, _buffer[i]);
    }
}

template <typename T, std::size_t sz>
void sizebounded<T,sz>::transform(std::function<T(const int, const T)> f)
{
    for (int i=0; i < sz; i++) {
        _buffer[i] = f(i, _buffer[i]);
    }
}

template <typename T1, typename T2, std::size_t sz1, std::size_t sz2>
bool memcopy(sizebounded<T2,sz2> &target, const sizebounded<T1,sz1> &source, std::size_t nsrc)
{
  std::size_t b1 = sizeof(T1);
  std::size_t b2 = sizeof(T2);
  if (nsrc < 1) { return false; }
  if (nsrc * b1 > sz2 * b2) { return false; }
  std::size_t minbytes = std::min(nsrc*b1, sz1*b1);
  if (minbytes % b2 != 0) { // adjust for target size
    minbytes = std::min(minbytes, (minbytes / b2) * b2); }
  auto res = memcpy((void*)(target.ptr()), (void*)(source.ptr()), minbytes);
#ifdef DEBUG
  if (getenv("PRINT_DEBUG") != NULL) {
    std::clog << "copied " << minbytes << " bytes" << std::endl;
    std::clog << "b2=" << b2 << " minbytes / b2=" << (minbytes / b2) << " minbytes % b2=" << (minbytes % b2) << std::endl;
    for (int i=0; i<minbytes; i++) {
      std::clog << "s    " << i << " " << int(*(((char*)source.ptr())+i)) << std::endl;
      std::clog << "t    " << i << " " << int(*(((char*)target.ptr())+i)) << std::endl;
    }
  }
#endif
  return res != nullptr;
}

