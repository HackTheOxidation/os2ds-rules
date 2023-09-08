#include <health_rule.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <string_view>

using namespace OS2DSRules::HealthRule;

class HealthRuleTest : public testing::Test {};

TEST_F(HealthRuleTest, Henlo_Test) {
  HealthRule rule;
  const std::string content = "Cancer er en grim sygdom";

  const auto result = rule.find_matches(content);

  ASSERT_EQ(2, result.size());
  ASSERT_EQ(std::string("cancer"), result[0].match());
  ASSERT_EQ(std::string("sygdom"), result[1].match());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
