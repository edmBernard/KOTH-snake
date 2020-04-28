// This is a simple HTTP(S) web server much like Python's SimpleHTTPServer

#include <string>

#include <cxxopts.hpp>
#include <uwebsockets/App.h>
#include <fmt/format.h>

/* Helpers for this example */
#include "helpers/AsyncFileReader.h"
#include "helpers/AsyncFileStreamer.h"


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

    uWS::App()
              .get("/api/register/:name", [&asyncFileStreamer](auto *res, auto *req) {
                std::cout << req->getParameter(0) << std::endl;
                res->writeHeader("Content-Type", "application/json")->end("{\"key\": \"XXX\", \"position\":{\"x\":1, \"y\": 1}}");
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
