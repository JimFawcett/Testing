#pragma once
/////////////////////////////////////////////////////////////////////////
// IQTestLogger.h - Queued Logger interface                            //
//                                                                     //
// Jim Fawcett, Emeritus Teaching Professor, EECS, Syracuse University //
/////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "ITestLogger.h"

namespace Test {
  template<typename T = std::string>
  struct IQTestLogger : public ITestLogger<T> {
    virtual ~IQTestLogger() {};
    virtual void wait() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual double elapsedMicroseconds() = 0;
  };
}