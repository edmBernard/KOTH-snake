#pragma once

#include <game/direction.hpp>

class Board {
  Board(int height, int width)
      : height(height), width(width) {
  }

  bool move(int x, int y, DIRECTION direction) {return true;}

private:
  int height;
  int width;
};
