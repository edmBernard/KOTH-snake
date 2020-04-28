
#include "catch.hpp"

#include <game/player.hpp>

TEST_CASE("Player", "[Board]") {
  States my_state(10, 10);
  SECTION("Add") {
    REQUIRE(my_state.getLength() == 0);
    my_state.addPlayer(5, 5);
    REQUIRE(my_state.getLength() == 1);
    my_state.addPlayer(5, 5);
    REQUIRE(my_state.getLength() == 2);
    my_state.addPlayer(5, 5);
    REQUIRE(my_state.getLength() == 3);
  }
  SECTION("Direction") {
    SECTION("1 player") {
      int player_idx = my_state.addPlayer(5, 5);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::NONE);
      my_state.setDirection(player_idx, DIRECTION::UP);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::UP);
    }
    SECTION("5 player") {
      my_state.addPlayer(5, 5);  // 0
      my_state.addPlayer(5, 5);  // 1
      my_state.addPlayer(5, 5);  // 2
      int player_idx = my_state.addPlayer(5, 5); // 3
      my_state.addPlayer(5, 5);  // 4
      REQUIRE(player_idx == 3);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::NONE);
      my_state.setDirection(player_idx, DIRECTION::UP);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::UP);
    }
  }
}
