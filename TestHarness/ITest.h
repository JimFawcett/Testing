#pragma once
///////////////////////////////////////////////////////////////////
// ITest.h - interface for Test Classes                          //
//                                                               //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syracuse Univ  //
///////////////////////////////////////////////////////////////////

#include <string>

namespace Test {

  struct ITest {
    virtual ~ITest() {}
    virtual bool test() = 0;
    virtual std::string name() = 0;
  };
}