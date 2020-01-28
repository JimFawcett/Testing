#pragma once
///////////////////////////////////////////////////////////////////
// TestUtilities.h - Display helper classes                      //
//                                                               //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syracuse Univ  //
///////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

namespace Test {

  inline void Assert(bool predicate, const std::string& message = "", size_t ln = 0, bool doThrow = false) {
    if (predicate)
      return;
    std::string sentMsg = "Assertion raised";
    if (ln > 0)
      sentMsg += " at line number " + std::to_string(ln);
    if (message.size() > 0)
      sentMsg += "\n  message: \"" + message + "\"";
    if (doThrow)
      throw std::exception(sentMsg.c_str());
    else
      std::cout << "\n  " + sentMsg;
  }

  inline void Requires(bool predicate, const std::string& message, size_t lineNo, bool doThrow = false) {
    if (predicate)
      return;
    std::string sentMsg = "Requires " + message + " raised";
    sentMsg += " at line number " + std::to_string(lineNo);
    if (doThrow)
      throw std::exception(sentMsg.c_str());
    else
      std::cout << "\n  " + sentMsg;
  }

  inline void Ensures(bool predicate, const std::string& message, size_t lineNo, bool doThrow = false) {
    if (predicate)
      return;
    std::string sentMsg = "Ensures " + message + " raised";
    sentMsg += " at line number " + std::to_string(lineNo);
    if (doThrow)
      throw std::exception(sentMsg.c_str());
    else
      std::cout << "\n  " + sentMsg;
  }
}