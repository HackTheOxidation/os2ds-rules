#include <cpr-detector.hpp>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace OS2DSRules::CPRDetector;

class CPRDetectorTest : public testing::Test {};

TEST_F(CPRDetectorTest, Test_Find_Basic_CPR_Number) {
  std::string content = "1111111118";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(1, results.size());
  ASSERT_STREQ("1111111118", results[0].match().c_str());
  ASSERT_EQ(0, results[0].start());
  ASSERT_EQ(9, results[0].end());
}

TEST_F(CPRDetectorTest, Test_Find_Basic_CPR_Number_With_Valid_Separators) {
  std::string content = "11 11 11 1118";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(1, results.size());
  ASSERT_STREQ("1111111118", results[0].match().c_str());
  ASSERT_EQ(0, results[0].start());
  ASSERT_EQ(12, results[0].end());
}

TEST_F(CPRDetectorTest, Test_Find_CPR_Number_With_Valid_Leap_Year) {
  std::string content = "2902081111";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(1, results.size());
  ASSERT_STREQ("2902081111", results[0].match().c_str());
}

TEST_F(CPRDetectorTest, Test_Find_CPR_Number_With_Modulus11_Check) {
  std::string content = "1111111118";
  CPRDetector detector(true);

  auto results = detector.find_matches(content);

  ASSERT_EQ(1, results.size());
  ASSERT_STREQ("1111111118", results[0].match().c_str());
}

TEST_F(CPRDetectorTest, Test_Find_CPR_Number_Tab_Newline_Separated) {
  std::string content = "111111\t1118\n111111\t0137\n111111\nBACON";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(2, results.size());
  ASSERT_STREQ("1111111118", results[0].match().c_str());
  ASSERT_STREQ("1111110137", results[1].match().c_str());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Too_Many_Separators) {
  std::string content = "111111  1118";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Invalid_Separator) {
  std::string content = "111111b1118";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Invalid_Prefix) {
  std::string content = "#1111111118";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Invalid_Suffix) {
  std::string content = "1111111118#";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Invalid_Date_Format) {
  std::string content = "9999999999";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Invalid_Date_February) {
  std::string content = "3002111111";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Invalid_Leap_Year) {
  std::string content = "2902111111";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Control_All_Zeros) {
  std::string content = "1111110000";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Modulus11_Check_Fail) {
  std::string content = "1111111111";
  CPRDetector detector(true);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

TEST_F(CPRDetectorTest, Test_Find_Two_CPR_Numbers_Separated_By_Whitespace) {
  std::string content = "1111111118 2304516782";
  CPRDetector detector(false);

  auto results = detector.find_matches(content);

  ASSERT_EQ(2, results.size());
  ASSERT_STREQ("1111111118", results[0].match().c_str());
  ASSERT_STREQ("2304516782", results[1].match().c_str());
}

TEST_F(CPRDetectorTest, Test_Reject_CPR_Number_With_Blacklisted_Words) {
  std::string content = "This is a fak-nr 1111111118";
  CPRDetector detector(false, true);

  auto results = detector.find_matches(content);

  ASSERT_EQ(0, results.size());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
