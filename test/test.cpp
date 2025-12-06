#include <catch2/catch_test_macros.hpp>
#include <iostream>

// change if you choose to use a different header name
#include "../src/CampusCompass.h"

using namespace std;

TEST_CASE("Basic Output Test, same as Gradescope #1") {


  string expectedOutput = R"(successfulsuccessfulsuccessfulsuccessfulunsuccessful2)";

  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  c.ParseCommand(R"(insert "Student A" 10000001 1 1 COP3502 )");
  c.ParseCommand(R"(insert "Student B" 10000002 1 1 COP3502 )");
  c.ParseCommand(R"(insert "Student C" 10000003 7 2 COP3502 MAC2311 )");
  c.ParseCommand(R"(dropClass 10000001 COP3502 )");
  c.ParseCommand(R"(remove 10000001 )");
  c.ParseCommand(R"(removeClass COP3502 )");

  string actualOutput = c.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);
}

TEST_CASE("Five Incorrect Commands") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  string expectedOutput = R"(unsuccessfulsuccessfulunsuccessfulunsuccessfulunsuccessfulunsuccessful)";


  c.ParseCommand(R"(insert "A11y" 45679999 1 1 COP3530)");
  c.ParseCommand(R"(insert "Dave" 12345678 1 1 COP3530)");
  c.ParseCommand(R"(insert "Dave Dupe" 12345678 1 1 COP3530)");
  c.ParseCommand(R"(insert "Many Classes Michael" 99999999 1 7 COP3530 MAC2312 MAC2311 MAC2313 PHY2048 PHY2049 IDS2935)");
  c.ParseCommand(R"(dropClass 12345678 cop3530)");
  c.ParseCommand(R"(insrt "Linda" 11111111 1 1 COP3530)");

  string actualOutput = c.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);

}

TEST_CASE("Three Edge Cases") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  string expectedOutput = R"(successfulunsuccessfulunsuccessfulsuccessfulunsuccessful)";


  c.ParseCommand(R"(insert "Dave" 12345678 1 1 COP3530)");
  c.ParseCommand(R"(remove 11111111)");
  c.ParseCommand(R"(replaceClass 12345678 MAC2312 MAC2311)");
  c.ParseCommand(R"(toggleEdgesClosure 1 33 52)");
  c.ParseCommand(R"(isConnected 33 52)");

  string actualOutput = c.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);

}

TEST_CASE("dropClass, removeClass, remove, and replaceClass") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  string expectedOutput = R"(successfulsuccessful1successful)";


  c.ParseCommand(R"(insert "Dave" 12345678 1 2 COP3530 MAC2312)");
  c.ParseCommand(R"(replaceClass 12345678 MAC2312 MAC2311)");
  c.ParseCommand(R"(removeClass MAC2311)");
  c.ParseCommand(R"(remove 12345678)");

  string actualOutput = c.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);

}

TEST_CASE("printShortestEdges (and printStudentZone) Test") {
  CampusCompass c;
  c.ParseCSV("data/edges.csv", "data/classes.csv");

  string expectedOutput = R"(successfulName: DaveCOP3530 | Total Time: 7MAC2312 | Total Time: 3Student Zone Cost For Dave: 10successfulName: DaveCOP3530 | Total Time: -1MAC2312 | Total Time: -1)";


  c.ParseCommand(R"(insert "Dave" 12345678 17 2 COP3530 MAC2312)");
  c.ParseCommand(R"(printShortestEdges 12345678)");
  c.ParseCommand(R"(printStudentZone 12345678)");
  c.ParseCommand(R"(toggleEdgesClosure 3 17 18 14 17 17 25)");
  c.ParseCommand(R"(printShortestEdges 12345678)");

  string actualOutput = c.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);

}




