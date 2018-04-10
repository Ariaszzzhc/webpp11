#ifndef WEBPP11_SERVER_H
#define WEBPP11_SERVER_H

#include <boost/asio.hpp>
// #include <boost/asio/ssl.hpp>
#include <regex>
#include <thread>

#include "webpp11/request.h"
#include "webpp11/response.h"

namespace webpp {

typedef boost::asio::ip::tcp::socket HTTP;
// typedef boost::asio::ssl::stream<HTTP> HTTPS;

typedef std::map<
    std::string,
    std::unordered_map<std::string,
                       std::function<std::shared_ptr<Response>(Request&)>>>
    Routes;

template <typename socket_type>
class ServerBase {
 public:
  ServerBase(unsigned short port, size_t num_threads = 1)
      : endpoint(boost::asio::ip::tcp::v4(), port),
        acceptor(io_service, endpoint),
        num_threads(num_threads) {}

  void start(Routes& routes) {
    for (auto it = routes.begin(); it != routes.end(); ++it) {
      all_routes.push_back(it);
    }

    accept();

    for (size_t i = 1; i < num_threads; ++i) {
      threads.emplace_back([this]() { io_service.run(); });
    }

    io_service.run();

    for (auto& t : threads) {
      t.join();
    }
  }

 protected:
  std::vector<Routes::iterator> all_routes;

  boost::asio::io_service io_service;
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;

  size_t num_threads;
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
    for (auto res_it : all_routes) {
      if (res_it->first == request->path) {
        if (res_it->second.count(request->method) > 0) {
          auto response = res_it->second[request->method](*request);
          auto write_buffer = response->get();

          boost::asio::async_write(
              *socket, *write_buffer,
              [this, socket, request, write_buffer](
                  const boost::system::error_code& ec,
                  size_t bytes_transferred) {
                if (!ec && std::stof(request->http_version) > 1.05)
                  process(socket);
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
    auto socket = std::make_shared<HTTP>(io_service);

    acceptor.async_accept(*socket,
                          [this, socket](const boost::system::error_code& e) {
                            accept();

                            if (!e) process(socket);
                          });
  }
};

// class SslServer : public ServerBase<HTTPS> {
//  public:
//   SslServer(unsigned short port, size_t num_threads = 1,
//             const std::string&& cert_file, const std::string&& private_key_file)
//       : ServerBase<HTTPS>::ServerBase(port, num_threads),
//         context(boost::asio::ssl::context::sslv23) {
//     context.use_certificate_chain_file(cert_file);
//     context.use_private_key_file(private_key_file,
//                                  boost::asio::ssl::context::pem);
//   }

//  private:
//   boost::asio::ssl::context context;

//   void accept() override {
//     auto socket = std::make_shared<HTTPS>(io_service, context);

//     acceptor.async_accept(
//         socket->lowest_layer(),
//         [this, socket](const boost::system::error_code& e) {
//           accept();

//           if (!e) {
//             socket->asnyc_handshake(
//                 boost::asio::ssl::stream_base::server,
//                 [this, socket](const boost::system::error_code& e) {
//                   if (!e) {
//                     process(socket);
//                   }
//                 });
//           }
//         });
//   }
// };
};  // namespace webpp

#endif