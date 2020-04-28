
#include "catch.hpp"

#include <game/player.hpp>

TEST_CASE("Board", "[Board]") {
  States my_state(10, 10);
  SECTION("Bundaries") {
    SECTION("UP") {
      DIRECTION direction = DIRECTION::UP;
      SECTION("FREE") {
        int x = 5;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 6);
        REQUIRE(y == 5);
      }
      SECTION("LIMIT") {
        int x = 10;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 10);
        REQUIRE(y == 5);
      }
      SECTION("OUT OF LIMIT") {
        int x = 20;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 10);
        REQUIRE(y == 5);
      }
    }
    SECTION("DOWN") {
      DIRECTION direction = DIRECTION::DOWN;
      SECTION("FREE") {
        int x = 5;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 4);
        REQUIRE(y == 5);
      }
      SECTION("LIMIT") {
        int x = 0;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 0);
        REQUIRE(y == 5);
      }
      SECTION("OUT OF LIMIT") {
        int x = -10;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 0);
        REQUIRE(y == 5);
      }
    }
    SECTION("RIGHT") {
      DIRECTION direction = DIRECTION::RIGHT;
      SECTION("FREE") {
        int x = 5;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 5);
        REQUIRE(y == 6);
      }
      SECTION("LIMIT") {
        int x = 5;
        int y = 10;
        my_state.move(x, y, direction);
        REQUIRE(x == 5);
        REQUIRE(y == 10);
      }
      SECTION("OUT OF LIMIT") {
        int x = 5;
        int y = 20;
        my_state.move(x, y, direction);
        REQUIRE(x == 5);
        REQUIRE(y == 10);
      }
    }
    SECTION("LEFT") {
      DIRECTION direction = DIRECTION::LEFT;
      SECTION("FREE") {
        int x = 5;
        int y = 5;
        my_state.move(x, y, direction);
        REQUIRE(x == 5);
        REQUIRE(y == 4);
      }
      SECTION("LIMIT") {
        int x = 5;
        int y = 0;
        my_state.move(x, y, direction);
        REQUIRE(x == 5);
        REQUIRE(y == 0);
      }
      SECTION("OUT OF LIMIT") {
        int x = 5;
        int y = -10;
        my_state.move(x, y, direction);
        REQUIRE(x == 5);
        REQUIRE(y == 0);
      }
    }

  }
}
