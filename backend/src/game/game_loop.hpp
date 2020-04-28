
#pragma once

#include <game/player.hpp>

#include <chrono> // std::chrono::seconds
#include <mutex>
#include <string>
#include <thread>
#include <vector>

inline void gameLoop(States &state, std::mutex &mx) {

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mx.lock();
    state.update();
    std::cout << state << std::endl;
    mx.unlock();
  }
}
