#pragma once
/////////////////////////////////////////////////////////////
// Testharness.h - single-user test sequence executor      //
//                                                         //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syr Univ //
/////////////////////////////////////////////////////////////
/*
   Package Responsibilities
  --------------------------
   Executes test sequences:
   - Executes bool test() method on each registered test class
   - Executes bool registeredFunction() for each registered function

   Package Dependencies:
  -----------------------
   TestHarness.h
   ITest.h

   Maintenance History:
  ----------------------
   ver 1.0 - 25 Jan 2020
   - first release
*/
#include <string>
#include <vector>
#include <iostream>
#include "ITest.h"

namespace Test {

  /*-- method pointer type declaration --*/
  template<typename T>
  using MP = bool(T::*)();

  /*-- function pointer type declaration --*/
  using FP = bool(*)();


  ///////////////////////////////////////////////
  // Executor class

  template<typename T>
  class Executor {
  public:
    Executor() = default;

    /*-- execute test class instance's test method --*/

    bool doTest(MP<T> tp, T* pT) {
      bool result = false;
      try {
        result = (pT->*tp)();
      }
      catch (...) {
        std::cout << "\n  exception thrown";
        result = false;
      }
      return result;
    }
    /*-- execute test function --*/

    bool doTest(FP fp) {
      bool result = false;
      try {
        result = fp();
      }
      catch (...) {
        std::cout << "\n  exception thrown";
        result = false;
      }
      return result;
    }
    /*-- report results to avoid repetition in test code --*/

    void showResult(bool r, const std::string& name) {
      if (r) {
        std::cout << "\n  " << name << " passed";
      }
      else {
        std::cout << "\n  " << name << " failed";
      }
    }
  };

  /*-- define collection of test class instances --*/
  template<typename T>
  using ClassTests = std::vector<T>;

  /*-- define collection of test function pointers --*/
  using FunctionTests = std::vector<std::pair<FP, std::string>>;

  template<typename T>
  class TestExecutive {
  public:
    /*-- register test class --*/
    void reg(T& t) {
      ctests_.push_back(std::move(t));
    }
    /*-- register test function --*/
    void reg(FP t, const std::string& name) {
      ftests_.push_back(std::pair{ t, name });
    }
    /*-- execute all registered tests --*/
    bool doTests() {
      Executor<T> ex;
      bool rtn = true;
      for (auto t : ftests_) {
        bool r = ex.doTest(t.first);
        ex.showResult(r, t.second);
        rtn &= r;
      }
      for (auto& t : ctests_) {
        bool r = t.test();
        ex.showResult(r, t.name());
        rtn &= r;
      }
      return rtn;
    }
  private:
    ClassTests<T> ctests_;
    FunctionTests ftests_;
  };

  /*-- display helper for function tests --*/
  inline void results(bool result, const std::string& msg = "") {
    std::cout << "\n  " << msg << " passed : " << result;
  }
}