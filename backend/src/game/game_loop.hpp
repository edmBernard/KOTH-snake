
#pragma once

#include <game/player.hpp>

#include <chrono> // std::chrono::seconds
#include <mutex>
#include <string>
#include <thread>
#include <vector>

inline void gameLoop(States &state, std::mutex &mx) {

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    mx.lock();
    state.update();
    mx.unlock();
  }
}
