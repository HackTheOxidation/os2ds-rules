#include <data_structures.hpp>
#include <array>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <string_view>

using namespace OS2DSRules::DataStructures;

class FrozenHashSetTest : public testing::Test {};

TEST_F(FrozenHashSetTest, Test_Contains) {
  FrozenHashSet<3> set(std::to_array<std::string_view>({"a", "b", "c"}));

  ASSERT_TRUE(set.contains("a"));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
