// This is a simple HTTP(S) web server much like Python's SimpleHTTPServer

#include <string>
#include <mutex>
#include <random>
#include <charconv>

#include <cxxopts.hpp>
#include <uwebsockets/App.h>
#include <fmt/format.h>

/* Helpers for this example */
#include "helpers/AsyncFileReader.h"
#include "helpers/AsyncFileStreamer.h"

#include <game/player.hpp>
#include <game/game_loop.hpp>

int main(int argc, char **argv) {
  try {
    // =================================================================================================
    // CLI
    cxxopts::Options options(argv[0], "A simple http server");
    options.positional_help("[optional args]").show_positional_help();

    // clang-format off
    options.add_options()("help", "Print help")
      ("p,port", "port", cxxopts::value<int>()->default_value("80"), "PORT")
      ("d,directory", "directory to serve", cxxopts::value<std::string>(), "DIRECTORY")
      ("w,width", "board game width", cxxopts::value<int>()->default_value("40"), "WIDTH")
      ("h,height", "board game height", cxxopts::value<int>()->default_value("10"), "HEIGHT")
      ;
    // clang-format on
    options.parse_positional({"directory"});
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
    }

    if (!result.count("directory")) {
      std::cout << "Error: directory argument required" << std::endl;
      return 1;
    }

    std::string root = result["directory"].as<std::string>();
    int port = result["port"].as<int>();

    AsyncFileStreamer asyncFileStreamer(root);


    States state(result["width"].as<int>(), result["height"].as<int>());


    std::mutex mx;
    std::thread logicLoop(gameLoop, std::ref(state), std::ref(mx));

    std::random_device rd;
    std::mt19937 randomGenerator(rd());
    std::uniform_int_distribution<int> distWidth(0, state.width - 1);
    std::uniform_int_distribution<int> distHeight(0, state.height - 1);

    uWS::App()
              .get("/api/register/:name/:color", [&state, &randomGenerator, &distWidth, &distHeight](auto *res, auto *req) {
                // TODO: do something with use the player name
                int x = distHeight(randomGenerator);
                int y = distWidth(randomGenerator);

                // convertion between string_view to int was ugly as fuck but don't find better way
                auto sv = req->getParameter(1);
                int color;
                auto result = std::from_chars(sv.data(), sv.data() + sv.size(), color);
                if (result.ec == std::errc::invalid_argument) {
                  // TODO: return proper error code
                  res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Bad color value");
                }

                int playerIdx = state.addPlayer(x, y, color);

                // TODO: add hash table to obfuscate player index in a hash or big number
                // TODO: add json output format currently key,x,y,color
                res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(fmt::format("{0},{1},{2},{3}", playerIdx, x, y, color));
              })
              .post("/api/status/:key", [&asyncFileStreamer](auto *res, auto *req) {
                std::cout << req->getParameter(0) << std::endl;
                res->writeHeader("Content-Type", "application/json")->end("{\"position\":{\"x\":1, \"y\": 1}}");
              })
              .post("/api/move/:key/:x/:y", [&asyncFileStreamer](auto *res, auto *req) {
                std::cout << req->getParameter(0) << std::endl;
                std::cout << req->getParameter(1) << std::endl;
                std::cout << req->getParameter(2) << std::endl;
                res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("success");
              })
              .post("/api/quit/:key", [&asyncFileStreamer](auto *res, auto *req) {
                std::cout << req->getParameter(0) << std::endl;
                res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("success");
              })
              .get("/api/*", [&asyncFileStreamer](auto *res, auto *req) {
                res->writeHeader("Content-Type", "application/json")->end("{\"hello\": \"world\"}");
              })
              // Static File
              .get("/static/:folder/:file", [&asyncFileStreamer](auto *res, auto *req) {
                asyncFileStreamer.streamFile(res, fmt::format("/{0}/{1}", req->getParameter(0), req->getParameter(1)));
                res->end();
              })
              // Default to home page
              .get("/*", [&asyncFileStreamer](auto *res, auto *req) {
                // You can efficiently stream huge files too
                asyncFileStreamer.streamFile(res, "/");
                res->end();
              })
        .listen(port, [port, root](auto *token) {
          if (token) {
            std::cout << "Serving " << root << " over HTTP a " << port << std::endl;
          }
        })
        .run();


  } catch (const cxxopts::OptionException &e) {
    std::cout << "Error: parsing options: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
