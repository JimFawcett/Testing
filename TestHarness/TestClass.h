#pragma once
///////////////////////////////////////////////////////////////////
// TestWidgetClass.h - Demonstration Test Class                  //
//                                                               //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syracuse Univ  //
///////////////////////////////////////////////////////////////////
/*
   Package Repsponsibilities:
  ----------------------------
   Provides class, TestWidgeClass, designed to test an instance 
   of another class, Widget. Both class names should change to
   suit the application.

   Package Dependencies:
  -----------------------
   TestWidgetClass.h, TestWidgetClass.cpp
   ITest.h
   Tested.h, Tested.cpp
   Testharness.h, TestHarness.cpp

   Maintenance History:
  ----------------------
   ver 1.0 : 25 Jan 2020
   - first release
*/
#include <memory>
#include "ITest.h"
#include "Tested.h"
#include "TestHarness.h"

namespace Test {
  /*-- test class must implement ITest --*/

  class TestWidgetClass : public ITest {
  public:
    /*-- create instance holding unique_ptr to tested class --*/
    TestWidgetClass() : pWidget_(testedCode::createWidget()) {}

    /*-- move required to use TestSequencer --*/
    TestWidgetClass(TestWidgetClass&& tc) noexcept {
      pWidget_ = std::move(tc.pWidget_);
    }
    /*---------------------------------------------
      test implementation classes are specific to
      each tested application
    */
    bool test1();
    bool test2();
    bool test3();
    bool test4();

    /*-- required overrides to implement ITest interface --*/
    virtual bool test() override;
    virtual std::string name() override {
      return "TestWidgetClass";
    }
  private:
    /*-- required to reference code to test --*/
    std::unique_ptr<testedCode::IWidget> pWidget_;

    /*------------------------------------------------------
      required to execute tests : template parameter is
      test class name
    */
    Executor<TestWidgetClass> executor_;
  };

  /*---------------------------------------------------------
    Test Description:
    - Demonstrate testing using test harness TestSequencer
    - Can be run outside the TestSequencer
    Test Environment:
    - All code built with C++17 option
    Test Operation:
    - run each of the implementing tests: test1 to test4
  */
  bool TestWidgetClass::test() {
    std::cout << "\n  Testing " << name();
    bool t1 = executor_.doTest(&TestWidgetClass::test1, this);
    executor_.showResult(t1, "test1");
    bool t2 = executor_.doTest(&TestWidgetClass::test2, this);
    executor_.showResult(t2, "test2");
    bool t3 = executor_.doTest(&TestWidgetClass::test3, this);
    executor_.showResult(t3, "test3");
    bool t4 = executor_.doTest(&TestWidgetClass::test4, this);
    executor_.showResult(t4, "test4");
    return t1 && t2 && t3 && t4;
  }
  /*-- Requirement #1 Widget Class --*/
  /*-- Widget is initialized with name = "unknown --*/
  bool TestWidgetClass::test1() {
    return (pWidget_->name() == "unknown");
  }

  /*---------------------------------------------------------
     Requirement #2 Widget Class
     - Widget::name(const std::string&) sets name_ member
     - Widget::name() returns value of name_ member
  */
  bool TestWidgetClass::test2() {
    pWidget_->name("testItem");
    return (pWidget_->name() == "testItem");
  }

  /*---------------------------------------------------------
     Requirement #3 Widget Class
     - Widget::say() returns
       "hi from Widget instance " + name_
     - Requires test2() to run immediately before this test
  */
  bool TestWidgetClass::test3() {
    std::string temp = pWidget_->say();
    return temp == "hi from Widget instance testItem";
  }

  /*---------------------------------------------------------
     Requirement #4 Executor
     - Tests Executor::doTest(), required to return false
       if exception is thrown during execution of test.
     - Also tests Executor::showResult(r, msg)
  */
  bool TestWidgetClass::test4() {
    throw(std::exception());
    return true;
  }
}