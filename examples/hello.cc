#include <webpp11/server.h>
#include <iostream>
#include <sstream>

using namespace webpp;
using namespace std;

int main() {
    HttpServer server(12345, 4);
    server.resource_["^//?$"]["GET"] = [](ostream& response, Request& request) {
        string hello = "Hello, World!";
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << hello.length()
                 << "\r\n\r\n"
                 << hello;
    };
    cout << "Server starting at port: 12345" << endl;
    server.start();

    return 0;
}
