#pragma once
///////////////////////////////////////////////////////////////////
// TestUtilities.h - Display helper classes                      //
//                                                               //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syracuse Univ  //
///////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

/*-- display Title with underline character '=' --*/
inline void Title(const std::string& title) {
  std::cout << "\n  " << title;
  std::cout << "\n " << std::string(title.size() + 2, '=');
}
/*-- display title with underline character '-' --*/
inline void title(const std::string& title) {
  std::cout << "\n  " << title;
  std::cout << "\n " << std::string(title.size() + 2, '-');
}
/*-- display indented string on next line --*/
inline void show(const std::string& arg) {
  std::cout << "\n  " << arg;
};
/*-- display indented new line with optional suffix*/
inline auto putline = [](size_t n = 1, const std::string& arg = "") {
  for (size_t i = 0; i < n; ++i)
    show(arg);
};
/*-- emit 2 newlines at end of program --*/
struct Cosmetic {
  ~Cosmetic() {
    std::cout << "\n\n";
  }
};