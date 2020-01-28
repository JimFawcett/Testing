#pragma once
/////////////////////////////////////////////////////////////////////////
// TestLogger.h - Logs to multiple streams                             //
//                                                                     //
// Jim Fawcett, Emeritus Teaching Professor, EECS, Syracuse University //
/////////////////////////////////////////////////////////////////////////
/*
   Package Responsibilities:
  ---------------------------
   Package provides ITestLogger interface, TestLogger class, and two factories:
   - Write log messages to multiple streams
   - Accept strings or messages convertible to string
   - TestLogger<T> provides:
     - post(msg) and postDated(msg)
     - addStream(pStrm), removeStream(pStrm), streamCount()
     - clear()
     - setPrefix(prfx) and setSuffix(suffx)

   Requires:
  -----------
   - C++17 option:
     Properties > C/C++ > Language > C++ Language Standard 
                > ISO C++17 standard (/std:c++17)
   - Catch C Exception:
     Properties > C/C++ > Code Generation > Enable C++ Exceptions 
                > Yes with SEH Exceptions (/EHa)

   Dependencies:
  ---------------
   ITestLogger.h
   TestLogger.h, TestLogger.cpp (only for demonstration)
   DateTime.h, DateTime.cpp
   TypeTraits.h

   Maintenance History:
  ----------------------
   ver 1.0 : 26 Jan 2020
   - first release
*/

#include "ITestLogger.h"
#include "../DateTime/DateTime.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>

namespace Test {
  /////////////////////////////////////////////////////////
  // TestLogger class

  template<typename T = std::string>
  class TestLogger : public ITestLogger<T> {
  public:
    using Streams = std::vector<std::ostream*>;

    TestLogger() {}
    virtual ~TestLogger();
    virtual void addStream(std::ostream* pOstream) override;
    virtual bool removeStream(std::ostream* pStrm) override;
    virtual void clear() override;
    virtual size_t streamCount() override;
    virtual void post(const T& msg) override;
    virtual void postDated(const T& msg) override;
    virtual void setPrefix(const std::string& prefix) override;
    virtual void setSuffix(const std::string& suffix) override;
  protected:
    void corePost(const T& msg);
    Streams streams_;
    std::string prefix_ = "\n  ";
    std::string suffix_ = "";
    Utilities::DateTime dt;
    std::string composite_;
  };

  /*-- remove all streams, closing file streams --*/
  template<typename T>
  TestLogger<T>::~TestLogger() {
    clear();
  }
  /*-- add ostream pointer, opens new log channel --*/
  template<typename T>
  void TestLogger<T>::addStream(std::ostream* pOstream) {
    streams_.push_back(pOstream);
  }
  /*-- remove ostream pointer, closes log channel --*/
  template<typename T>
  bool TestLogger<T>::removeStream(std::ostream* pStrm) {
    try {
      std::ofstream* pFile = dynamic_cast<std::ofstream*>(pStrm);
      if (pFile != nullptr) {
        std::cout << "\n -- closing file --";
        pFile->close();
        std::cout.flush();
      }
    }
    catch (std::exception & ex) {
      std::cout << "\n--" << ex.what();
    }
    catch (...) {
      std::cout << "\n -- SEH exception";
      // Stream in heap already deleted by std::unique_ptr
      // That happens when logger goes out of scope
    }
    Streams::iterator iter = std::find(streams_.begin(), streams_.end(), pStrm);
    if (iter == streams_.end())
      return false;
    streams_.erase(iter);
    return true;
  }
  /*-- remove all streams, reset prefix and suffix --*/
  template<typename T>
  void TestLogger<T>::clear() {
    for (auto pStrm : streams_)
      removeStream(pStrm);
    prefix_ = "\n  ";
    suffix_ = "";
  }
  /*-- return number of open log channels --*/
  template<typename T>
  size_t TestLogger<T>::streamCount() {
    return streams_.size();
  }
  /*-- private write log message to all channels --*/
  template<typename T>
  void TestLogger<T>::corePost(const T& msg) {
    composite_ = prefix_ + msg + suffix_;
    for (auto pStrm : streams_) {
      (*pStrm) << composite_;
    }
  }
  /*-- write log message to all channels --*/
  template<typename T>
  void TestLogger<T>::post(const T& msg) {
    corePost(msg);
  }
  /*-- write dated log message to all channels --*/
  template<typename T>
  void TestLogger<T>::postDated(const T& msg) {
    composite_ = msg + " : " + dt.now();
    corePost(composite_);
  }
  /*-- set new message prefix --*/
  template<typename T>
  void TestLogger<T>::setPrefix(const std::string& prefix) {
    prefix_ = prefix;
  }
  /*-- set new message suffix --*/
  template<typename T>
  void TestLogger<T>::setSuffix(const std::string& suffix) {
    suffix_ = suffix;
  }

  /////////////////////////////////////////////////
  // Logger factory functions
  // - return pointer or reference typed as ITestLogger<T> interface

  /*-- return std::unique_ptr<IQTestLogger> bound to a new instance --*/

  template<typename T = std::string>
  inline std::unique_ptr<ITestLogger<T>> createLogger(std::ostream* pStrm = &std::cout) {
    auto pLogger = std::unique_ptr<TestLogger<std::string>>(new TestLogger<std::string>());
    pLogger->addStream(pStrm);
    return pLogger;
  }

  template<typename T = std::string>
  inline ITestLogger<T>& getSingletonLogger(std::ostream* pStrm = &std::cout) {
    static TestLogger<T> logger;
    if (logger.streamCount() == 0)
      logger.addStream(pStrm);
    return logger;
  }

  /////////////////////////////////////////////////
  // Open file helper methods

  /*-----------------------------------------------
    - opens filestream passed as pointer
    - user required to declare std::ofstream
      and pass its address
  */
  inline bool openFile(const std::string& filename, std::ofstream* pOstrm) {
    pOstrm->open(filename, std::ios::out);
    return pOstrm->good();
  }
  /*-----------------------------------------------
    - open filestream and return std::pair of
      - bool success
      - std::unique_ptr<std::ofstream> to stream
        in native heap
      - allows user to open file with just name
      - user does not need to delete pointer
  */
  using FileReturn = std::pair<bool, std::unique_ptr<std::ofstream>>;

  inline FileReturn openFile(const std::string& filename) {
    std::ofstream* pFile = new std::ofstream(filename, std::ios::out);
    return std::pair{ pFile->good(), std::unique_ptr<std::ofstream>(pFile) };
  }

  /////////////////////////////////////////////////
  // LogMessage class
  // - required to have cast to string method

  class LogMessage : public std::string {
  public:
    LogMessage(const std::string& preamble) {
      *this += preamble;
    }
    LogMessage& add(const std::string& content) {
      *this += content;
      return *this;
    }
    operator std::string() {
      return static_cast<std::string>(*this);
    }
    std::string toString() {
      return static_cast<std::string>(*this);
    }
    void clear() {
      *this = static_cast<LogMessage>("");
    }
  };
}