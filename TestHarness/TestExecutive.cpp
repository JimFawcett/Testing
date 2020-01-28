///////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Assembles tests and starts execution      //
//                                                               //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syracuse Univ  //
///////////////////////////////////////////////////////////////////

#include "TestClass.h"
#include "Tested.h"
#include "Testharness.h"
#include "../TestUtilities/TestUtilities.h"

using namespace testedCode;
using namespace Test;

/*---------------------------------------------------------
  TestSequencer will run this function
---------------------------------------------------------*/

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
/*-- Widget is initialized with name = "inknown --*/
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

#ifdef TEST_TESTCLASS

bool testTester() {
  std::cout << "\n  testing tester";
  return true;
}
bool alwaysFails() {
  return false;
}

Cosmetic c;

int main() {

  Title("Testing TestClass");

  std::cout << std::boolalpha;
  TestWidgetClass tc;
  bool t = tc.test();
  if (t == false) {
    std::cout << "\n\n  at least one test failed";
  }
  else {
    std::cout << "\n\n  all tests passed";
  }
  putline();

  title("Testing test functions");

  Executor<ITest> executor;

  bool ta = executor.doTest(testTester);
  executor.showResult(ta, "testTester");
  bool tb = executor.doTest(alwaysFails);
  executor.showResult(tb, "alwaysFails");
  putline(1);

  title("Testing TestSequencer");

  TestWidgetClass tc2;
  TestSequencer<TestWidgetClass> te;
  te.reg(tc2);
  te.reg(testTester, "testTester");
  te.reg(alwaysFails, "alwaysFails");
  te.doTests();
}
#endif

