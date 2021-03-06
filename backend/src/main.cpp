// This is a simple HTTP(S) web server much like Python's SimpleHTTPServer

#include <string>
#include <mutex>
#include <random>

#include <cxxopts.hpp>
#include <uwebsockets/App.h>
#include <fmt/format.h>

/* Helpers for this example */
#include <helpers/AsyncFileReader.h>
#include <helpers/AsyncFileStreamer.h>


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
    uWS::App app;

    // TODO: add mutex inside State to lock writing/reading operation
    std::thread logicLoop(gameLoop, std::ref(state), std::ref(mx), std::ref(app));

    std::random_device rd;
    std::mt19937 randomGenerator(rd());
    std::uniform_int_distribution<int> distWidth(0, state.width - 1);
    std::uniform_int_distribution<int> distHeight(0, state.height - 1);

    struct PerSocketData {};

    app.get("/api/register/:name/:color", [&state, &mx, &randomGenerator, &distWidth, &distHeight](auto *res, auto *req) {
        // TODO: do something with use the player name
        int x = distHeight(randomGenerator);
        int y = distWidth(randomGenerator);

        // convertion between string_view to int was ugly as fuck but don't find better way
        auto sv = req->getParameter(1);
        int color;
        auto result = std::from_chars(sv.data(), sv.data() + sv.size(), color);
        if (result.ec == std::errc::invalid_argument) {
          res->writeStatus("400");
          res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Bad color value");
          return;
        }
        mx.lock();
        int playerIdx = state.addPlayer(x, y, color);
        mx.unlock();

        // TODO: add hash table to obfuscate player index in a hash or big number
        // TODO: add json output format currently key,x,y,color
        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(fmt::format("{0},{1},{2},{3}", playerIdx, x, y, color));
      })


      .get("/api/status/:key", [&state](auto *res, auto *req) {
        std::cout << req->getParameter(0) << std::endl;
        // TODO: add hash table to obfuscate player index in a hash or big number

        auto key_variant = state.validateKey(req->getParameter(0));
        if (key_variant.index() == 1) {
          res->writeStatus("400");
          res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(std::get<std::string>(key_variant));
          return;
        }
        int key = std::get<int>(key_variant);
        res->writeHeader("Content-Type", "application/json")->end(fmt::format("{0},{1},{2}", state.positionsX[key], state.positionsY[key], state.colors[key]));
      })


      .post("/api/move/:key/:direction", [&state, &mx](auto *res, auto *req) {

        auto key_variant = state.validateKey(req->getParameter(0));
        if (key_variant.index() == 1) {
          res->writeStatus("400");
          res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(std::get<std::string>(key_variant));
          return;
        }
        int key = std::get<int>(key_variant);

        auto dir_variant = state.validateDirection(req->getParameter(1));
        if (dir_variant.index() == 1) {
          res->writeStatus("400");
          res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(std::get<std::string>(dir_variant));
          return;
        }
        int direction = std::get<DIRECTION>(dir_variant);

        mx.lock();
        state.setDirection(key, static_cast<DIRECTION>(direction));
        mx.unlock();

        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("success");
      })


      .get("/api/map/buffer", [&state](auto *res, auto *req) {
        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(state.serializeMap());
      })

      .get("/api/map/size", [&state](auto *res, auto *req) {
        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(fmt::format("{0},{1}", state.width, state.height));
      })

      .get("/api/map/image", [&state](auto *res, auto *req) {
        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Not implemented yet");
      })


      .get("/api/players/buffer", [&state](auto *res, auto *req) {
        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end(state.serializePlayers());
      })


      .post("/api/quit/:key", [](auto *res, auto *req) {
        std::cout << req->getParameter(0) << std::endl;
        res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Not Implemented yet");
      })


      .get("/api/*", [](auto *res, auto *req) {
        res->writeHeader("Content-Type", "application/json")->end("{\"hello\": \"world\"}");
      })


      // Static File
      .get("/static/:folder/:file", [&asyncFileStreamer](auto *res, auto *req) {
        asyncFileStreamer.streamFile(res, fmt::format("/{0}/{1}", req->getParameter(0), req->getParameter(1)));
        res->end();
      })
      // Default to home page
      .get("/", [&asyncFileStreamer](auto *res, auto *req) {
        // You can efficiently stream huge files too
        asyncFileStreamer.streamFile(res, "/");
        res->end();
      })
      .get("/*", [](auto *res, auto *req) {
        res->writeStatus("404");
        res->end("Not Found");
      })

      .ws<PerSocketData>("/*", {
        .open = [](auto *ws, auto *req) {
          ws->subscribe("update");
        },
        .close = [](auto *ws, int code, std::string_view message) {
        }
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
