// ======================================================================
// \title  LedTestMain.cpp
// \author aidandb
// \brief  cpp file for Led component test main function
// ======================================================================

#include "LedTester.hpp"

TEST(Nominal, toDo) {
  Components::LedTester tester;
  tester.testBlinking();
  tester.testBlinkInterval();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
