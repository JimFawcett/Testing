#pragma once
/////////////////////////////////////////////////////////////////////////
// QTestLogger.h - Logs to multiple streams using post queue           //
//                                                                     //
// Jim Fawcett, Emeritus Teaching Professor, EECS, Syracuse University //
/////////////////////////////////////////////////////////////////////////
/*
   Package Responsibilities:
  ---------------------------
   Package provides IQTestLogger interface, QTestLogger class, and two factories:
   - Write log messages to multiple streams
   - Accept strings or messages convertible to string
   QTestLogger<T> posts to write queue.  Child thread deQs and writes to streams.
   - QTestLogger<T> provides:
     - post(msg) and postDated(msg)
     - addStream(pStrm), removeStream(pStrm), streamCount()
     - clear()
     - start(), stop(), and elapsedMicroseconds()
     - wait()
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
   IQTestLogger.h
   QTestLogger.h
   ITestLogger.h
   TestLogger.h, TestLogger.cpp (only for demonstration)
   DateTime.h, DateTime.cpp
   TypeTraits.h

   Maintenance History:
  ----------------------
   ver 1.0 : 26 Jan 2020
   - first release, 
     starting new version change due to redesign
*/

#include "IQTestLogger.h"
#include "TestLogger.h"
#include "../DateTime/DateTime.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../type_traits/TypeTraits.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <memory>

namespace Test {

  /////////////////////////////////////////////////////////
  // QTestLogger class

  template<typename T = std::string>
  class QTestLogger : public IQTestLogger<T>, public TestLogger<T> {
  public:

    QTestLogger() {
      wthread = std::move(std::thread(&QTestLogger<T>::writeThreadProc, this));
    }
    virtual ~QTestLogger();
    virtual void wait();
    virtual void start();
    virtual void stop();
    virtual double elapsedMicroseconds();
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
    std::thread wthread;
    BlockingQueue<T> writeQ_;
    void writeThreadProc();
  };

  /*-- remove all streams, closing file streams --*/
  template<typename T>
  QTestLogger<T>::~QTestLogger() {
    writeQ_.enQ("stop");
    if (wthread.joinable())
      wthread.join();
    clear();
  }
  /*-- wait for writeQ to empty --*/
  template<typename T>
  void QTestLogger<T>::wait() {
    while (writeQ_.size() > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  /*-- start timer --*/
  template<typename T>
  void QTestLogger<T>::start() {
    this->dt.start();
  }
  /*-- stop timer --*/
  template<typename T>
  void QTestLogger<T>::stop() {
    this->dt.stop();
  }
  /*-- timer elapsed microseconds --*/
  template<typename T>
  double QTestLogger<T>::elapsedMicroseconds() {
    return this->dt.elapsedMicroseconds();
  }

  template<typename T>
  void QTestLogger<T>::addStream(std::ostream* pOstream) {
    TestLogger<T>::addStream(pOstream);
  }

  template<typename T>
  bool QTestLogger<T>::removeStream(std::ostream* pStrm) {
    return TestLogger<T>::removeStream(pStrm);
  }
  /*-- return number of open log channels --*/
  template<typename T>
  size_t QTestLogger<T>::streamCount() {
    return TestLogger<T>::streamCount();
  }
  /*-- remove all streams, reset prefix and suffix --*/
  template<typename T>
  void QTestLogger<T>::clear() {
    wait();
    for (auto pStrm : TestLogger<T>::streams_)
      this->removeStream(pStrm);
    TestLogger<T>::prefix_ = "\n  ";
    TestLogger<T>::suffix_ = "";
  }
  /*-- function executed by write thread --*/
  template<typename T>
  void QTestLogger<T>::writeThreadProc() {
    while (true) {
      T msg = writeQ_.deQ();
      if (msg == "stop")
        break;
      for (auto pStrm : TestLogger<T>::streams_) {
        (*pStrm) << msg;
      }
    }
  }
  /*-- write log message to all channels --*/
  template<typename T>
  void QTestLogger<T>::corePost(const T& msg) {
    this->composite_ = this->prefix_ + msg + this->suffix_;
    writeQ_.enQ(this->composite_);
  }
  /*-- write log message to all channels --*/
  template<typename T>
  void QTestLogger<T>::post(const T& msg) {
    corePost(msg);
  }
  /*-- write dated log message to all channels --*/
  template<typename T>
  void QTestLogger<T>::postDated(const T& msg) {
    this->composite_ = msg + " : " + this->dt.now();
    corePost(this->composite_);
  }
  /*-- set prefix applied to all messages --*/
  template<typename T>
  void QTestLogger<T>::setPrefix(const std::string& prefix) {
    TestLogger<T>::setPrefix(prefix);
  }
  /*-- set suffix applied to all messages --*/
  template<typename T>
  void QTestLogger<T>::setSuffix(const std::string& suffix) {
    TestLogger<T>::setSuffix(suffix);
  }

  /////////////////////////////////////////////////
  // Logger factory functions
  // - return pointer or reference typed as IQTestLogger<T> interface

  /*-- return std::unique_ptr<IQTestLogger> bound to a new instance --*/
  template<typename T = std::string>
  inline std::unique_ptr<IQTestLogger<T>> createQLogger(std::ostream* pStrm = &std::cout) {
    auto pQLogger = std::unique_ptr<QTestLogger<T>>(new QTestLogger<T>());
    pQLogger->addStream(pStrm);
    return pQLogger;
  }
  /*-- return reference to single static instance of logger --*/
  template<typename T = std::string>
  inline IQTestLogger<T>& getSingletonQLogger(std::ostream* pStrm = &std::cout) {
    static QTestLogger<T> logger;
    if (logger.streamCount() == 0)
      logger.addStream(pStrm);
    return logger;
  }
}