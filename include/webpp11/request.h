#ifndef WEBPP11_REQUEST_H
#define WEBPP11_REQUEST_H

#include <string>
#include <unordered_map>

namespace webpp {
struct Request {
  std::string method, path, http_version;
  std::shared_ptr<std::istream> content;
  std::unordered_map<std::string, std::string> header;
};
}  // namespace webpp

#endif