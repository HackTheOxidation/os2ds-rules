#include <data_structures.hpp>
#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace OS2DSRules::DataStructures;

class HashMapTest : public testing::Test {};

TEST_F(HashMapTest, Test_Default_HashMap_Is_Empty) {
  HashMap<int, int> hm;

  ASSERT_EQ(0, hm.size());
  ASSERT_EQ(5, hm.capacity());
}

TEST_F(HashMapTest, Test_Insert_Increases_Size_By_One) {
  HashMap<int, int> hm;
  hm.insert(1, 1);

  ASSERT_EQ(1, hm.size());
  ASSERT_EQ(5, hm.capacity());
}

TEST_F(HashMapTest, Test_Insert_One) {
  HashMap<int, int> hm;
  hm.insert(1, 1);

  ASSERT_EQ(1, hm.size());
  ASSERT_TRUE(hm.contains(1));
}

TEST_F(HashMapTest, Test_Default_Does_Not_Contain_1) {
  HashMap<int, int> hm;

  ASSERT_EQ(0, hm.size());
  ASSERT_FALSE(hm.contains(1));
}

TEST_F(HashMapTest, Test_Insert_One_Does_Not_Contain_Two) {
  HashMap<int, int> hm;
  hm.insert(1, 1);

  ASSERT_EQ(1, hm.size());
  ASSERT_EQ(5, hm.capacity());
  ASSERT_TRUE(hm.contains(1));
  ASSERT_FALSE(hm.contains(2));
}

TEST_F(HashMapTest, Test_Insert_Remove_Is_Empty) {
  HashMap<int, int> hm;
  hm.insert(1, 1);

  ASSERT_EQ(1, hm.size());
  ASSERT_TRUE(hm.contains(1));

  hm.remove(1);

  ASSERT_EQ(0, hm.size());
  ASSERT_FALSE(hm.contains(1));
}

TEST_F(HashMapTest, Test_Insert_Triggers_Rehashing) {
  HashMap<int, int> hm(2);
  hm.insert(1, 1);
  hm.insert(2, 2);

  ASSERT_EQ(2, hm.size());
  ASSERT_EQ(2, hm.capacity());

  hm.insert(3, 3);

  ASSERT_EQ(3, hm.size());
  ASSERT_EQ(4, hm.capacity());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
