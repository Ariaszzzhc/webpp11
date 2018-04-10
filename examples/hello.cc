#include <webpp11/app.h>

using namespace webpp;
using namespace std;

int main() {
  HttpApplication app(12345, 4);

  app.add_route("/", [](Request& request) {
    auto response = make_shared<Response>("Hello, World!");

    return response;
  });

  app.run();

  return 0;
}