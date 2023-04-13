#include <address_rule.hpp>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace OS2DSRules::AddressRule;

class AddressRuleTest : public testing::Test {};

TEST_F(AddressRuleTest, Test_Empty_Content_No_Matches) {
  AddressRule rule;
  auto results = rule.find_matches("");

  ASSERT_EQ(0, results.size());
}

TEST_F(AddressRuleTest, Test_Address_Name_Only_No_Matches) {
  AddressRule rule;
  auto results = rule.find_matches("Aabyvej");

  ASSERT_EQ(0, results.size());
}

TEST_F(AddressRuleTest, Test_Address_And_Number_Matches) {
  AddressRule rule;
  auto results = rule.find_matches("Aabyvej 1");

  ASSERT_EQ(1, results.size());
  ASSERT_EQ(std::string("Aabyvej 1"), results[0].match());
  ASSERT_EQ(0, results[0].start());
  ASSERT_EQ(8, results[0].end());
}

TEST_F(AddressRuleTest, Test_Multiword_Address_And_Number_Matches) {
  AddressRule rule;
  auto results = rule.find_matches("Aabenraa Landevej 1");

  ASSERT_EQ(1, results.size());
  ASSERT_EQ(std::string("Aabenraa Landevej 1"), results[0].match());
}

TEST_F(AddressRuleTest, Test_Address_Invalid_Number_No_Matches) {
  AddressRule rule;
  auto results = rule.find_matches("Aabyvej 01");

  ASSERT_EQ(0, results.size());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
