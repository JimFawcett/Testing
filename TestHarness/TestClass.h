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
}