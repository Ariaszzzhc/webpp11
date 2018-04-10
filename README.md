# Webpp11
Simple Web Framework



## Feature

C++ 11

Headers Only

Simple

Just for fun



## How to use

```c++
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
```



## More

Still in development...