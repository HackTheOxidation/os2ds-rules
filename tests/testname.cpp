#include <name_rule.hpp>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace OS2DSRules::NameRule;

class NameRuleTest : public testing::Test {};

TEST_F(NameRuleTest, Test_Empty_Content_No_Matches) {
  NameRule rule;
  auto results = rule.find_matches("");

  ASSERT_EQ(0, results.size());
}

TEST_F(NameRuleTest, Test_Single_Name_One_Match) {
  NameRule rule;
  auto results = rule.find_matches("John");

  ASSERT_EQ(1, results.size());
  ASSERT_EQ(std::string("John"), results[0].match());
}

TEST_F(NameRuleTest, Test_Double_Name_Single_Match) {
  NameRule rule;
  auto results = rule.find_matches("John Peter");

  ASSERT_EQ(1, results.size());
  ASSERT_EQ(std::string("John Peter"), results[0].match());
}

TEST_F(NameRuleTest, Test_Double_Name_Single_Name_Two_Matches) {
  NameRule rule;
  auto results = rule.find_matches("John Peter is usually just called John.");

  ASSERT_EQ(2, results.size());
  ASSERT_EQ(std::string("John Peter"), results[0].match());
  ASSERT_EQ(std::string("John"), results[1].match());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
