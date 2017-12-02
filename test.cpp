/*
 *
 *    on bad access an exception is raised
 *    g++-6 -o test test.cpp -std=c++17 -Wall
 *
 *    turning off exceptions results in a SIGSEGV
 *    g++-6 -o test test.cpp -std=c++17 -Wall -fno-exceptions
 */

#include <iostream>
#include "sizebounded.hpp"
#include "sizebounded.ipp"

int main(int argc, char *argv[])
{
  sizebounded<unsigned char, 3> buf3;

  buf3[0] = 'x';
  buf3[1] = 'l';
  buf3[2] = 'd';

  const unsigned char c = buf3[2];

  int i = 0;
  for (auto c : buf3) {
    std::clog << "@ " << i++ << " " << c << std::endl;
  }
  
  return 0;
}

