
#include "catch.hpp"

#include <game/player.hpp>

TEST_CASE("Player", "[Player]") {
  States my_state(10, 10);
  SECTION("Add") {
    REQUIRE(my_state.getLength() == 0);
    my_state.addPlayer(5, 5, 0);
    REQUIRE(my_state.getLength() == 1);
    my_state.addPlayer(5, 5, 0);
    REQUIRE(my_state.getLength() == 2);
    my_state.addPlayer(5, 5, 0);
    REQUIRE(my_state.getLength() == 3);
  }
  SECTION("Direction") {
    SECTION("1 player") {
      int player_idx = my_state.addPlayer(5, 5, 0);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::NONE);
      my_state.setDirection(player_idx, DIRECTION::UP);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::UP);
    }
    SECTION("5 player") {
      my_state.addPlayer(5, 5, 0);  // 0
      my_state.addPlayer(5, 5, 0);  // 1
      my_state.addPlayer(5, 5, 0);  // 2
      int player_idx = my_state.addPlayer(5, 5, 0); // 3
      my_state.addPlayer(5, 5, 0);  // 4
      REQUIRE(player_idx == 3);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::NONE);
      my_state.setDirection(player_idx, DIRECTION::UP);
      REQUIRE(my_state.getDirection(player_idx) == DIRECTION::UP);
    }
  }
}

TEST_CASE("Update", "[State]") {
  int dim = 10;
  States my_state(dim, dim);
  int player1 = my_state.addPlayer(0, 0, 1);
  int player2 = my_state.addPlayer(9, 0, 2);
  int player3 = my_state.addPlayer(9, 9, 3);
  int player4 = my_state.addPlayer(0, 9, 4);


  for (int i = 0; i < 12; ++i) {
    my_state.setDirection(player1, DIRECTION::DOWN);
    my_state.setDirection(player2, DIRECTION::RIGHT);
    my_state.setDirection(player3, DIRECTION::UP);
    my_state.setDirection(player4, DIRECTION::LEFT);
    {
      auto [x, y] = my_state.getPosition(player1);
      REQUIRE(x == (i > dim - 1 ? dim - 1 : i));
      REQUIRE(y == 0);
    }
    {
      auto [x, y] = my_state.getPosition(player2);
      REQUIRE(x == 9);
      REQUIRE(y == (i > dim - 1 ? dim - 1 : i));
    }
    {
      auto [x, y] = my_state.getPosition(player3);
      REQUIRE(x == (dim - 1 - i < 0 ? 0 : dim - 1 - i));
      REQUIRE(y == 9);
    }
    {
      auto [x, y] = my_state.getPosition(player4);
      REQUIRE(x == 0);
      REQUIRE(y == (dim - 1 - i < 0 ? 0 : dim - 1 - i));
    }
    my_state.update();
  }
}

TEST_CASE("Color", "[State]") {
  int dim = 10;
  States my_state(dim, dim);
  SECTION("1 player 1 movement") {
    int player1 = my_state.addPlayer(0, 0, 1);
    my_state.setDirection(player1, DIRECTION::DOWN);
    for (int i = 0; i < 12; ++i) {
      auto [x, y] = my_state.getPosition(player1);
      REQUIRE(my_state.board[my_state.offset(x, y)] == 1);
      my_state.update();
    }
    REQUIRE(my_state.board[my_state.offset(0, 0)] == 1);
    REQUIRE(my_state.board[my_state.offset(1, 0)] == 1);
    REQUIRE(my_state.board[my_state.offset(2, 0)] == 0);
  }
  SECTION("2 player 1 movement") {
    int player1 = my_state.addPlayer(0, 0, 1);
    int player2 = my_state.addPlayer(9, 0, 2);

    my_state.setDirection(player1, DIRECTION::DOWN);
    my_state.setDirection(player2, DIRECTION::RIGHT);

    for (int i = 0; i < 12; ++i) {
      {
        auto [x, y] = my_state.getPosition(player1);
        REQUIRE(my_state.board[my_state.offset(x, y)] == 1);
      }
      {
        auto [x, y] = my_state.getPosition(player2);
        REQUIRE(my_state.board[my_state.offset(x, y)] == 2);
      }
      my_state.update();
    }

    REQUIRE(my_state.board[my_state.offset(0, 0)] == 1);
    REQUIRE(my_state.board[my_state.offset(1, 0)] == 1);
    REQUIRE(my_state.board[my_state.offset(2, 0)] == 0);

    REQUIRE(my_state.board[my_state.offset(9, 0)] == 2);
    REQUIRE(my_state.board[my_state.offset(9, 1)] == 2);
    REQUIRE(my_state.board[my_state.offset(9, 2)] == 0);
  }
  SECTION("1 player many movement") {
    int player1 = my_state.addPlayer(0, 0, 1);
    for (int i = 0; i < 12; ++i) {
      my_state.setDirection(player1, DIRECTION::DOWN);

      auto [x, y] = my_state.getPosition(player1);
      REQUIRE(my_state.board[my_state.offset(x, y)] == 1);
      my_state.update();
    }

    for (int i = 0; i < 9; ++i) {
      REQUIRE(my_state.board[my_state.offset(i, 0)] == 1);
    }
  }
}