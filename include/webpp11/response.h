#ifndef WEBPP11_RESPONSE_H
#define WEBPP11_RESPONSE_H

#include <boost/asio.hpp>
#include "webpp11/status.h"

namespace webpp {
class Response {
 private:
  std::shared_ptr<std::ostream> response_stream;
  std::shared_ptr<boost::asio::streambuf> write_buffer;

 public:
  Response(const std::string& content, size_t length, const HttpStatus status) {
    write_buffer = std::make_shared<boost::asio::streambuf>();
    response_stream =
        std::make_shared<std::ostream>(write_buffer.get());

    *response_stream << "HTTP/1.1 " << status << " " << HttpStatusMap[status]
                     << "\r\nContent-Length: " << length << "\r\n\r\n" << content;
  }
  std::shared_ptr<boost::asio::streambuf> get() { return write_buffer; }
};
}  // namespace webpp

#endif