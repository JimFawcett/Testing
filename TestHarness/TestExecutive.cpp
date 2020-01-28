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

