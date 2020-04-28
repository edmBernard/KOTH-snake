
#pragma once

#include <game/direction.hpp>

#include <helpers/convertion.hpp>

#include <iostream>
#include <variant>
#include <vector>

// I try to made a data oriented design. I'm not fan, maybe I'm doing it wrong.
class States {
public:
  States(int width, int height)
      : width(width), height(height), board(height * width, 0) {
  }

  int addPlayer(int x, int y, int color) {
    positionsX.push_back(x);
    positionsY.push_back(y);
    colors.push_back(color);
    directions.push_back(DIRECTION::NONE);
    board[offset(x, y)] = color;
    // return player index we currently don't support deletion
    return getLength() - 1;
  }

  void setDirection(int playerIdx, DIRECTION direction) {
    directions[playerIdx] = direction;
  }

  DIRECTION getDirection(int playerIdx) {
    return directions[playerIdx];
  }

  std::pair<int, int> getPosition(int playerIdx) {
    return {positionsX[playerIdx], positionsY[playerIdx]};
  }

  void move(int &x, int &y, DIRECTION direction) {
    // Direction image convention
    // x positif is down
    // y positif is right

    switch (direction) {
    case DIRECTION::NONE:
      // TODO: Add test
      break;
    case DIRECTION::DOWN:
      x += 1;
      if (x > height - 1)
        x = height - 1;
      break;
    case DIRECTION::UP:
      x -= 1;
      if (x < 0)
        x = 0;
      break;
    case DIRECTION::RIGHT:
      y += 1;
      if (y > width - 1)
        y = width - 1;
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
    return colors.size();
  };

  void update() {
    for (auto i = 0; i < getLength(); ++i) {
      // update position
      move(positionsX[i], positionsY[i], directions[i]);

      // update board color on position
      board[offset(positionsX[i], positionsY[i])] = colors[i];

      // reset all direction
      directions[i] = DIRECTION::NONE;
    }
    epoch++;
  }

  // compute offset to convert (x,y) in buffer position
  int offset(int x, int y) {
    return y + x * width;
  }

  std::variant<int, std::string> validateKey(const std::string_view &key) {
    auto result = convert<int>(key);
    if (!result) {
      return "Bad key format";
    }
    if (result.value() < 0 || result.value() > getLength() - 1) {
      return "Don't try to hack!";
    }
    return result.value();
  }

  std::variant<DIRECTION, std::string> validateDirection(const std::string_view &dir) {
    auto result = convert<int>(dir);
    if (!result) {
      return "Bad direction format";
    }
    if (result.value() > 5 || result.value() < 0) {
      return "Invalid value";
    }
    return static_cast<DIRECTION>(result.value());
  }


  long epoch = 0;
  int width;
  int height;
  std::vector<int> positionsX;
  std::vector<int> positionsY;
  std::vector<int> colors;
  std::vector<DIRECTION> directions;
  std::vector<int> board;
};

inline std::ostream &operator<<(std::ostream &os, const States &state) {
  for (size_t i = 0; i < state.board.size(); ++i) {
    os << state.board[i] << " ";
    if ((i + 1) % state.width == 0) {
      os << "\n";
    }
  }
  return os;
}