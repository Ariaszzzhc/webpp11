#include <webpp11/app.h>

using namespace webpp;
using namespace std;

int main() {
  HttpApplication app(12345, 4);

  app.add_route("/", [](ostream& response, Request& request) {
    string hello = "Hello, World!";
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << hello.length()
             << "\r\n\r\n"
             << hello;
  });

  app.run();

  return 0;
}