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

  app.add_route("/", [](ostream& response, Request& request) {
    string hello = "Hello, World!";
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << hello.length()
             << "\r\n\r\n"
             << hello;
  });

  app.run();

  return 0;
}
```



## More

Still in development...