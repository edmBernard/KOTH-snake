
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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    mx.lock();
    state.update();
    mx.unlock();
    app.publish(std::string_view("update"), state.serializeMap(), uWS::TEXT);
  }
}
