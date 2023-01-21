#include <gtest/gtest.h>
#include "main.cpp"


TEST(HelloTest, fight_test) {

  //Hero hero("Taras", 20, 2);
  //Dragon dragon("Scylla", 30, 3, 0);

  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
