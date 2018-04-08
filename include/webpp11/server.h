#ifndef WEBPP11_SERVER_H
#define WEBPP11_SERVER_H

#include <boost/asio.hpp>
#include <regex>
#include <thread>
#include <unordered_map>

namespace webpp {

typedef boost::asio::ip::tcp::socket HTTP;

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
  Resource resource_;
  Resource default_resource_;

  ServerBase(unsigned short port, size_t num_threads = 1)
      : endpoint_(boost::asio::ip::tcp::v4(), port),
        acceptor_(io_service_, endpoint_),
        num_threads_(num_threads) {}
  void start() {
    for (auto it = resource_.begin(); it != resource_.end(); ++it) {
      all_reource_.push_back(it);
    }

    for (auto it = default_resource_.begin(); it != default_resource_.end(); ++it) {
      all_reource_.push_back(it);
    }

    accept();

    for (size_t i = 1; i < num_threads_; ++i) {
      threads.emplace_back([this]() { io_service_.run(); });
    }

    io_service_.run();

    for (auto& t : threads) {
      t.join();
    }
  }

 protected:
  std::vector<Resource::iterator> all_reource_;

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::endpoint endpoint_;
  boost::asio::ip::tcp::acceptor acceptor_;

  size_t num_threads_;
  std::vector<std::thread> threads;

  virtual void accept() = 0;
  void process(std::shared_ptr<socket_type> socket) const {
    auto read_buffer = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(
        *socket, *read_buffer, "\r\n\r\n",
        [this, socket, read_buffer](const boost::system::error_code& e,
                                    size_t bytes_transferred) {
          if (!e) {
            size_t total = read_buffer->size();

            std::istream stream(read_buffer.get());

            auto request = std::make_shared<Request>();
            *request = parse_request(stream);

            size_t num_additional_bytes = total - bytes_transferred;

            if (request->header.count("Content-Length") > 0) {
              boost::asio::async_read(
                  *socket, *read_buffer,
                  boost::asio::transfer_exactly(
                      std::stoull(request->header["Content-Length"]) -
                      num_additional_bytes),
                  [this, socket, read_buffer, request](
                      const boost::system::error_code& e,
                      size_t byte_transferred) {
                    if (!e) {
                      request->content = std::shared_ptr<std::istream>(
                          new std::istream(read_buffer.get()));
                      respond(socket, request);
                    }
                  });
            } else {
              respond(socket, request);
            }
          }
        });
  }

  void respond(std::shared_ptr<socket_type> socket,
               std::shared_ptr<Request> request) const {
    for (auto res_it : all_reource_) {
      std::regex e(res_it->first);
      std::smatch sm_res;

      if (std::regex_match(request->path, sm_res, e)) {
        if (res_it->second.count(request->method) > 0) {
          request->path_match = move(sm_res);

          auto write_buffer = std::make_shared<boost::asio::streambuf>();
          std::ostream response(write_buffer.get());
          res_it->second[request->method](response, *request);

          boost::asio::async_write(
              *socket, *write_buffer,
              [this, socket, request, write_buffer](
                  const boost::system::error_code& ec,
                  size_t bytes_transferred) {
                // HTTP 持久连接(HTTP 1.1), 递归调用
                if (!ec && stof(request->http_version) > 1.05) process(socket);
              });
          return;
        }
      }
    }
  }

  Request parse_request(std::istream& stream) const {
    Request request;
    std::unordered_map<std::string, std::string> header;

    std::regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch sub_match;

    std::string line;
    getline(stream, line);
    line.pop_back();

    if (std::regex_match(line, sub_match, e)) {
      request.method = sub_match[1];
      request.path = sub_match[2];
      request.http_version = sub_match[3];

      bool matched;
      e = "^([^:]*): ?(.*)$";
      do {
        getline(stream, line);
        line.pop_back();
        matched = std::regex_match(line, sub_match, e);
        if (matched) {
          request.header[sub_match[1]] = sub_match[2];
        }
      } while (matched);
    }

    return request;
  }
};

class HttpServer : public ServerBase<HTTP> {
 public:
  HttpServer(unsigned short port, size_t num_threads = 1)
      : ServerBase<HTTP>::ServerBase(port, num_threads) {}

 private:
  void accept() override {
    auto socket = std::make_shared<HTTP>(io_service_);

    acceptor_.async_accept(*socket,
                           [this, socket](const boost::system::error_code& e) {
                             accept();

                             if (!e) process(socket);
                           });
  }
};
};  // namespace webpp

#endif