
#include "catch.hpp"

#include <game/player.hpp>

TEST_CASE("Player", "[Board]") {
  States my_state(10, 10);
  SECTION("Add") {
    REQUIRE(my_state.getLength() == 0);
    my_state.add(5, 5);
    REQUIRE(my_state.getLength() == 1);
    my_state.add(5, 5);
    REQUIRE(my_state.getLength() == 2);
    my_state.add(5, 5);
    REQUIRE(my_state.getLength() == 3);
  }
}