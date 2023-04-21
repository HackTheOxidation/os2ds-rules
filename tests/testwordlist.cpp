#include <wordlist_rule.hpp>

#include <array>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <string_view>

using namespace OS2DSRules::WordListRule;

class WordListRuleTest : public testing::Test {};

TEST_F(WordListRuleTest, Test_Find_Simple_Word) {
  auto words = std::to_array<std::string_view>({
      "Hello",
      "World",
  });

  WordListRule rule(words.begin(), words.end());

  auto results = rule.find_matches("Hello, World!");

  ASSERT_EQ(2, results.size());
  ASSERT_EQ(std::string("Hello"), results[0].match());
  ASSERT_EQ(std::string("World"), results[1].match());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
