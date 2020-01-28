///////////////////////////////////////////////////////////////
// Tested.cpp - used to demonstrate testing with TestHarness //
//                                                           //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syr Univ   //
///////////////////////////////////////////////////////////////

#include "Tested.h"
#include <iostream>
#include "../TestUtilities/TestUtilities.h"

using namespace testedCode;

#ifdef TEST_TESTED
int main() {

  Widget widget1, widget2("Two");
  widget1.name("one");
  show(widget1.say());
  show(widget2.say());
  putline();

  std::unique_ptr<IWidget> pWidget = createWidget("Frank");
  show(pWidget->say());
  show(pWidget->name());
  pWidget->name("George");
  show(pWidget->say());
  show(pWidget->name());
  putline(2);
}
#endif
