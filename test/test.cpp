#include <catch2/catch_test_macros.hpp>
#include <iostream>

// change if you choose to use a different header name
#include "../src/CampusCompass.h"

using namespace std;

TEST_CASE("Example CampusC  ompass Output Test") {


  string expectedOutput = R"(successfulsuccessfulsuccessfulsuccessfulunsuccessful2)";

  CampusCompass c;
  c.ParseCommand(R"(insert "Student A" 10000001 7 1 COP3502)");
  c.ParseCommand(R"(insert "Student B" 10000002 7 1 COP3502)");
  c.ParseCommand(R"(insert "Student C" 10000003 7 2 COP3502 MAC2311)");
  c.ParseCommand(R"(dropClass 10000001 COP3502)");
  c.ParseCommand(R"(remove 10000001)");
  c.ParseCommand(R"(removeClass COP3502)");

  string actualOutput = c.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);
}
