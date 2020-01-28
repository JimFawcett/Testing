/////////////////////////////////////////////////////////////////////////
// TestLogger.cpp - Logging to multiple streams                        //
//                  Demonstrates TestLogger<T> and QTestLogger<T>      //
//                                                                     //
// Jim Fawcett, Emeritus Teaching Professor, EECS, Syracuse University //
/////////////////////////////////////////////////////////////////////////

#include "TestLogger.h"
#include "QTestLogger.h"
#include "../TestUtilities/TestAssertions.h"
#include "../Display/Display.h"

int main() {

  using namespace Test;

  displayTitle("Testing Logger");

  displayDemo("-- logging to std::cout --");
  TestLogger<std::string> logger;
  logger.addStream(&std::cout);
  logger.postDated("Test log #1");
  logger.post("first message");
  logger.post("second message");
  logger.post("");

  logger.setPrefix("\n  -- ");
  logger.setSuffix(" --");
  logger.postDated("Test log #2");
  logger.post("a message");
  std::cout << "\n";

  std::cout << "\n  logger has " << logger.streamCount() << " streams";
  logger.clear();
  std::cout << "\n  after clearing logger has " << logger.streamCount() << " streams";

  displayDemo("\n  -- logging LogMessages --");
  logger.addStream(&std::cout);
  LogMessage msg("github message");
  logger.postDated(msg);
  msg.clear();
  msg.add("send message").add("/with another part").add("/and still another part");
  logger.post(msg);

  displayDemo("\n  -- logging to file stream --");
  std::ofstream oStrm;
  if (openFile("test.log", &oStrm)) {
    std::cout << "\n  opened file \"test.log\"";
    logger.addStream(&oStrm);
    logger.postDated("logging to std::cout and file test.log");
    logger.post("end of test log");
  }
  std::cout << "\n  after leaving openFile scope, streamCount = " << logger.streamCount();
  logger.post("posting after leaving addStream scope");
  logger.clear();

  displayDemo("\n  -- logging to std::cout and two file streams --");
  logger.addStream(&std::cout);
  std::ofstream strm2;
  if (openFile("newtest.log", &strm2)) {
    logger.addStream(&strm2);
  }
  FileReturn fr = openFile("newertest.log");
  auto pStrm = std::move(fr.second);
  if (fr.first) {
    if (pStrm->good())
      logger.addStream(pStrm.get());
  }
  logger.setPrefix("\n  ");
  logger.setSuffix("");
  logger.post("posting after leaving addStream with unique_ptr scope");
  logger.clear();
  std::cout << "\n";

  displayDemo("-- use factory and interface --");
  auto pLogger = createLogger();
  pLogger->postDated("factory logger log");
  pLogger->post("log message #1");
  pLogger->setPrefix("\n --");
  pLogger->setSuffix(" --");
  pLogger->post("message with new prefix and suffix");
  putline();

  displayTitle("Testing QLogger");

  displayDemo("-- logging to std::cout --");
  QTestLogger<std::string> qlogger;
  qlogger.addStream(&std::cout);
  qlogger.postDated("Test log #1");
  qlogger.post("first message");
  qlogger.post("second message");
  qlogger.post("");

  qlogger.setPrefix("\n  -- ");
  qlogger.setSuffix(" --");
  qlogger.postDated("Test log #2");
  qlogger.post("a message");
  qlogger.wait();
  std::cout << "\n";

  std::cout << "\n  qlogger has " << qlogger.streamCount() << " streams";
  qlogger.clear();
  std::cout << "\n  after clearing qlogger has " << qlogger.streamCount() << " streams";

  displayDemo("\n  -- logging LogMessages --");
  qlogger.addStream(&std::cout);
  LogMessage qMsg("github message");
  qlogger.postDated(qMsg);
  qMsg.clear();
  qMsg.add("send message").add("/with another part").add("/and still another part");
  qlogger.post(qMsg);
  qlogger.wait();

  displayDemo("\n  -- logging to file stream --");
  std::ofstream oQStrm;
  if (openFile("test.log", &oQStrm)) {
    std::cout << "\n  opened file \"test.log\"";
    qlogger.addStream(&oQStrm);
    qlogger.postDated("logging to std::cout and file test.log");
    qlogger.post("end of test log");
  }
  qlogger.wait();
  std::cout << "\n  after leaving openFile scope, streamCount = " << qlogger.streamCount();
  qlogger.post("posting after leaving addStream scope");
  qlogger.clear();

  displayDemo("\n  -- logging to std::cout and two file streams --");
  qlogger.addStream(&std::cout);
  std::ofstream qstrm2;
  if (openFile("newtest.log", &qstrm2)) {
    qlogger.addStream(&qstrm2);
  }
  FileReturn qfr = openFile("newertest.log");
  auto pQStrm = std::move(qfr.second);
  if (qfr.first) {
    if (pQStrm->good())
      qlogger.addStream(pQStrm.get());
  }
  qlogger.setPrefix("\n  ");
  qlogger.setSuffix("");
  qlogger.post("posting after leaving addStream with unique_ptr scope");
  qlogger.clear();
  std::cout << "\n";

  displayDemo("-- use factory and interface --");
  auto pQlogger = createQLogger();
  pQlogger->postDated("factory Qlogger log");
  pQlogger->post("log message #1");
  pQlogger->setPrefix("\n --");
  pQlogger->setSuffix(" --");
  pQlogger->post("message with new prefix and suffix");
  pQlogger->wait();
  putline();

  displayDemo("-- use singleton factory and interface --");
  auto& qSlogger = getSingletonQLogger();
  qSlogger.postDated("factory qSlogger log");
  qSlogger.post("log message #1");
  qSlogger.setPrefix("\n --");
  qSlogger.setSuffix(" --");
  qSlogger.post("message with new prefix and suffix");
  qSlogger.wait();
  putline();

  displayDemo("-- use timer --");
  pQlogger->setPrefix("\n  ");
  pQlogger->setSuffix("");
  std::ofstream tfstrm;
  openFile("timed.log", &tfstrm);
  pQlogger->addStream(&tfstrm);
  pQlogger->post("starting timer");
  pQlogger->start();
  pQlogger->post("measuring time for this post");
  double d = pQlogger->elapsedMicroseconds();
  std::string dStr = std::to_string(d);
  size_t index = dStr.find('.');
  dStr = dStr.substr(0, index);
  pQlogger->post("elapsed time in microsec = " + dStr);
  pQlogger->wait();
  std::cout << "\n  after posting and waiting:";
  std::cout << "\n  elapsed microsecs = " << pQlogger->elapsedMicroseconds();
  putline(2);
}