#include <gtest/gtest.h>
#include "Characters.h"

TEST(HelloTest, fight_test) {

  Hero h("Taras", 20, 2);

  //Hero hero("Taras", 20, 2);
  //Dragon dragon("Scylla", 30, 3, 0);

  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
