#ifndef WEBPP11_SERVER_H
#define WEBPP11_SERVER_H

#include <boost/asio.hpp>
#include <regex>
#include <thread>
#include <unordered_map>

namespace webpp {
struct Request {
  std::string method, path, http_version;
  std::shared_ptr<std::istream> content;
  std::unordered_map<std::string, std::string> header;
  std::smatch path_match;
};

typedef std::map<std::string,
                 std::unordered_map<
                     std::string, std::function<void(std::ostream&, Request&)>>>
    Resource;

template <typename socket_type>
class ServerBase {
 public:
  Resource resource;
  Resource default_resource;

  ServerBase(unsigned short port, size_t num_threads = 1)
      : endpoint_(boost::asio::ip::tcp::v4(), port),
        acceptor_(io_service_, endpoint_),
        num_threads_(num_threads) {}
  void start();

 protected:
  std::vector<Resource::iterator> all_reource;

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::endpoint endpoint_;
  boost::asio::ip::tcp::acceptor acceptor_;

  size_t num_threads_;
  std::vector<std::thread> threads;

  virtual void accept();
  void process(std::shared_ptr<socket_type> socket) const;
};
};  // namespace webpp

#endif