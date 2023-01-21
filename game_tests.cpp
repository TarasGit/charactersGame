#include <gtest/gtest.h>
#include "Characters.h"

TEST(HelloTest, fight_dragon) {

  Hero hero("Taras", 20, 2);
  Dragon dragon("Dragon", 40, 4,0 );

  hero.attack(dragon);

  //EXPECT_STRNE("hello", "world");

  EXPECT_EQ(dragon.get_health(), 38);
}


TEST(HelloTest, fight_orc) {

  Hero hero("Taras", 20, 5);
  Orc orc("Orc", 10, 10,0 );

  hero.attack(orc);

  //EXPECT_STRNE("hello", "world");

  EXPECT_EQ(orc.get_health(), 5);
}
