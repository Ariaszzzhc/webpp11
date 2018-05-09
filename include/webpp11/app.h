#ifndef WEBPP11_APP_H
#define WEBPP11_APP_H

#include <iostream>
#include "webpp11/server.h"

namespace webpp {

class Application {
 private:
  Server server;
  Routes routes;
  unsigned short port;

 public:
  void add_route(const std::string url,
                 const std::function<std::shared_ptr<Response>(Request&)> func,
                 const std::string method = "GET") {
    routes[url][method] = func;
  }

  Application(const unsigned short port, const size_t num_threads = 1)
      : port(port), server(port, num_threads) {}

  void run() {
    std::cout << "Server starting at port: " << port << std::endl;
    server.start(routes);
  }
};

// class SslApplication : public ApplicationBase {
//  private:
//   SslServer server;

//  public:
//   SslApplication(const unsigned short port, const size_t num_threads = 1)
//       : ApplicationBase(port), server(port, num_threads) {}

//   void run() override {
//     std::cout << "Server starting at port: " << port << std::endl;
//     server.start(routes);
//   }
// };
}  // namespace webpp
#endif