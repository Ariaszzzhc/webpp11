#ifndef WEBPP11_APP_H
#define WEBPP11_APP_H

#include <iostream>
#include "webpp11/server.h"

namespace webpp {
class HttpApplication {
 private:
  Routes routes;
  HttpServer server;
  unsigned short port;

 public:
  void add_route(const std::string url,
                const std::function<void(std::ostream&, Request&)> func,
                const std::string method = "GET") {
    routes[url][method] = func;
  }
  HttpApplication(const unsigned short port, const size_t num_threads = 1)
      : server(port, num_threads), port(port) {}

  void run() {
    std::cout << "Server starting at port: " << port << std::endl;
    server.start(routes);
  }
};
}  // namespace webpp
#endif