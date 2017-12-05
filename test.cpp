/*
 *
 *    on bad access an exception is raised
 *    g++ -o test test.cpp -std=c++11 -Wall
 *
 *    turning off exceptions results in a SIGSEGV
 *    g++ -o test test.cpp -std=c++11 -Wall -fno-exceptions
 *    (or clang++)
 */

#define BOOST_TEST_MAIN

#include "boost/test/unit_test.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <iostream>
#include "sizebounded.hpp"
#include "sizebounded.ipp"

using namespace boost::posix_time;

BOOST_AUTO_TEST_SUITE( BoundedSizeUT )

BOOST_AUTO_TEST_CASE( Deep_Copy )
{
    setenv("PRINT_DEBUG", "yes", 1);
    constexpr int n = 1000;
    sizebounded<unsigned char,n> buf;
    buf[0]='a';
    buf[n-1]='z';
    {
      auto buf2 = buf;
      BOOST_CHECK_EQUAL(buf2[0], 'a');
      BOOST_CHECK_EQUAL(buf2[n-1], 'z');
    }
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf[n-1], 'z');
    std::clog << "all done." << std::endl;
}

BOOST_AUTO_TEST_CASE( String_Copy )
{
    setenv("PRINT_DEBUG", "yes", 1);
    constexpr int n = 20;
    sizebounded<unsigned char,n> buf;
    for (int i=0; i<n; i++) {
      buf[i]='a'+i; }
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf[n-1], 't');
    std::clog << "string copy: " << buf.toString() << std::endl;
}

BOOST_AUTO_TEST_CASE( Vector_Copy )
{
    setenv("PRINT_DEBUG", "yes", 1);
    constexpr int n = 20;
    sizebounded<unsigned char,n> buf;
    for (int i=0; i<n; i++) {
      buf[i]='a'+i; }
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf[n-1], 't');
    auto v = buf.toVector();
    BOOST_CHECK_EQUAL(v.size(), n);
    std::clog << "vector copy: " << typeid(v).name() << std::endl;
}

BOOST_AUTO_TEST_CASE( String_Copy_of_Int_Buffer )
{
    setenv("PRINT_DEBUG", "yes", 1);
    constexpr int n = 20;
    sizebounded<int,n> buf;
    for (int i=0; i<n; i++) {
      buf[i]='a'+i; }
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf[n-1], 't');
    std::string str = buf.toString();
    BOOST_CHECK_EQUAL(str.size(), n);
    std::clog << "string copy: " << str << std::endl;
}

BOOST_AUTO_TEST_CASE( Vector_Copy_of_Int_Buffer )
{
    setenv("PRINT_DEBUG", "yes", 1);
    constexpr int n = 20;
    sizebounded<int,n> buf;
    for (int i=0; i<n; i++) {
      buf[i]='a'+i; }
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf[n-1], 't');
    auto v = buf.toVector();
    BOOST_CHECK_EQUAL(v.size(), n);
    std::clog << "vector copy: " << typeid(v).name() << std::endl;
}

BOOST_AUTO_TEST_CASE( Timing_Basic )
{
    unsetenv("PRINT_DEBUG");
    std::clog << "Timing non-checked access:  ";
    constexpr int n = 100000;
    constexpr int r = 1000;
    ptime t0(microsec_clock::universal_time());
    for (int i = 0; i < r; i++) {
      unsigned char buf[n];
      for (int j = 0; j < n; j++) {
        buf[j] = j % 7;
      }
      for (auto c : buf) {
        c * 1;
      }
    }
    ptime t1(microsec_clock::universal_time());
    std::clog << "total time: " <<  double( (t1 - t0).total_microseconds() ) / 1000.0 << " ms" << std::endl;
}

BOOST_AUTO_TEST_CASE( Timing_Bounded )
{
    unsetenv("PRINT_DEBUG");
    std::clog << "Timing size-checked access: ";
    constexpr int n = 100000;
    constexpr int r = 1000;
    ptime t0(microsec_clock::universal_time());
    for (int i = 0; i < r; i++) {
      sizebounded<unsigned char, n> buf;
      for (int j = 0; j < n; j++) {
        buf[j] = j % 7;
      }
      for (auto c : buf) {
        c * 1;
      }
    }
    ptime t1(microsec_clock::universal_time());
    std::clog << "total time: " <<  double( (t1 - t0).total_microseconds() ) / 1000.0 << " ms" << std::endl;
}

BOOST_AUTO_TEST_CASE( map_function_over_buffer )
{
    constexpr int n = 20;
    sizebounded<char,n> buf;
    for (int i=0; i<n; i++) {
      buf[i]='a'; }
    auto buf2 = buf;
    // the underlying data structure is const
    buf.map([&buf2](int i, char c) {
        buf2[i] = c + 1;
      });
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf2[0], 'b');
    std::clog << "buf =  " << buf.toString() << std::endl;
    std::clog << "buf2 = " << buf2.toString() << std::endl;
}

BOOST_AUTO_TEST_CASE( transform_buffer_functionally )
{
    constexpr int n = 20;
    sizebounded<char,n> buf;
    for (int i=0; i<n; i++) {
      buf[i]='a'+i; }
    auto buf2 = buf;
    // in-place transformation of elements
    buf2.transform([](int i, char c)->char {
        if (i % 2 == 0) {
          return c + 1;
        } else {
          return c;
        }
      });
    BOOST_CHECK_EQUAL(buf[0], 'a');
    BOOST_CHECK_EQUAL(buf2[0], 'b');
    std::clog << "buf =  " << buf.toString() << std::endl;
    std::clog << "buf2 = " << buf2.toString() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()


