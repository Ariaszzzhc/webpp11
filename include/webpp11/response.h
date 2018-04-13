#ifndef WEBPP11_RESPONSE_H
#define WEBPP11_RESPONSE_H

#include <boost/asio.hpp>
#include "webpp11/status.h"

namespace webpp {
class Response {
 private:
  std::shared_ptr<std::ostream> response_stream;
  std::shared_ptr<boost::asio::streambuf> write_buffer;
  http::HttpStatus status;

 public:
  Response(const std::string&& content,
           const http::HttpStatus status = http::OK)
      : status(status) {
    auto len = content.length();
    write_buffer = std::make_shared<boost::asio::streambuf>();
    response_stream = std::make_shared<std::ostream>(write_buffer.get());

    *response_stream << "HTTP/1.1 " << status << " "
                     << http::HttpStatusMap[status]
                     << "\r\nContent-Length: " << len << "\r\n\r\n"
                     << content;
  }
  inline std::shared_ptr<boost::asio::streambuf> get_buffer() { return write_buffer; }
  inline http::HttpStatus get_status() { return status; };
};
}  // namespace webpp

#endif