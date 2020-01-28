#pragma once
/////////////////////////////////////////////////////////////////////////
// ITestLogger.h - Logger interface                                    //
//                                                                     //
// Jim Fawcett, Emeritus Teaching Professor, EECS, Syracuse University //
/////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

namespace Test {
  template<typename T = std::string>
  struct ITestLogger {
    virtual ~ITestLogger() {}
    virtual void addStream(std::ostream* pOstream) = 0;
    virtual bool removeStream(std::ostream* pOstream) = 0;
    virtual size_t streamCount() = 0;
    virtual void post(const T& msg) = 0;
    virtual void postDated(const T& msg) = 0;
    virtual void setPrefix(const std::string& prefix) = 0;
    virtual void setSuffix(const std::string& suffix) = 0;
    virtual void clear() = 0;
  };
}