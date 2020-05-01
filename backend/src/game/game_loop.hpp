
#pragma once

#include <game/player.hpp>
#include <uwebsockets/App.h>

#include <chrono> // std::chrono::seconds
#include <mutex>
#include <string>
#include <thread>
#include <vector>

inline void gameLoop(States &state, std::mutex &mx, uWS::App &app) {

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    mx.lock();
    state.update();
    mx.unlock();
    app.publish(std::string_view("update"), std::string_view("update"), uWS::TEXT);
  }
}
