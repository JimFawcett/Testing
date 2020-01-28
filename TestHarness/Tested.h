#pragma once
/////////////////////////////////////////////////////////////
// Tested.h - used to demonstrate testing with TestHarness //
//                                                         //
// Jim Fawcett, Teaching Professor Emeritus, ECE, Syr Univ //
/////////////////////////////////////////////////////////////

#include <string>
#include <memory>
#include <iostream>

namespace testedCode{

  /*-------------------------------------------------
    IWidget required for TestWidgetClass to bind to 
    code to test 
  */
  struct IWidget {
    virtual ~IWidget() {}
    virtual std::string say() = 0;
    virtual void name(const std::string& name) = 0;
    virtual std::string name() = 0;
  };

  /*-- code being tested, just a simple demo class --*/

  class Widget : public IWidget {
  public:
    Widget() = default;

    Widget(const std::string& name) : name_(name) {}

    ~Widget() {
      std::cout << "\n  Widget " << name_ << " destroyed";
    }

    std::string say() override {
      return "hi from Widget instance " + name_;
    }
    void name(const std::string& name) override {
      name_ = name;
    }
    std::string name() override {
      return name_;
    }
  private:
    std::string name_ = "unknown";
  };

  /*-----------------------------------------------------
    Tested code requires:
    - an interface, e.g., IWidget
    - a factory function, e.g., createWidget, below
    - an implementation for the factory to create.
  */
  inline std::unique_ptr<IWidget> 
  createWidget(const std::string& name = "unknown") {
    return std::unique_ptr<Widget>(new Widget(name));
  }
}