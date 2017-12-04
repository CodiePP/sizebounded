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

BOOST_AUTO_TEST_CASE( Timing_Basic )
{
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

BOOST_AUTO_TEST_SUITE_END()


