
#pragma once

#include <game/direction.hpp>

#include <vector>

class States {
public:
  States(int height, int width)
      : height(height), width(width) {
  }

  int addPlayer(int x, int y) {
    positionsX.push_back(x);
    positionsY.push_back(y);
    colors.push_back(0);
    directions.push_back(DIRECTION::NONE);
    // return player index
    return getLength() - 1;
  }

  void setDirection(int playerIdx, DIRECTION direction) {
    directions[playerIdx] = direction;
  }

  DIRECTION getDirection(int playerIdx) {
    return directions[playerIdx];
  }

  void move(int &x, int &y, DIRECTION direction) {
    switch (direction) {
    case DIRECTION::NONE:
      // TODO: Add test
      break;
    case DIRECTION::UP:
      x += 1;
      if (x > height)
        x = height;
      break;
    case DIRECTION::DOWN:
      x -= 1;
      if (x < 0)
        x = 0;
      break;
    case DIRECTION::RIGHT:
      y += 1;
      if (y > width)
        y = width;
      break;
    case DIRECTION::LEFT:
      y -= 1;
      if (y < 0)
        y = 0;
      break;
    }

    // x = x > height ? height : x // TODO: Is it faster ?
    // x = x+1 > height ? height : x+1 // TODO: Is it faster ?
    // x += x+1 > height ? 0 : 1 // TODO: Is it faster ? Not exactly the good behavior
  }

  inline int getLength() {
    // TODO: Add test
    return colors.size();
  };


  void update() {
    // TODO:: Add test
    for (auto i = 0; i < getLength(); ++i) {
      move(positionsX[i], positionsY[i], directions[i]);
    }
    epoch++;
  }

private:
  long epoch = 0;
  int height;
  int width;
  std::vector<int> positionsX;
  std::vector<int> positionsY;
  std::vector<int> colors;
  std::vector<DIRECTION> directions;
};
