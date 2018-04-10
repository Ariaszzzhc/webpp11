#ifndef WEBPP11_STATUS_H
#define WEBPP11_STATUS_H

#include <string>
#include <map>

namespace webpp {
enum HttpStatus {
  Continue = 100,
  SwitchingProtocols = 101,
  OK = 200,
  Created = 201,
  Accepted = 202,
  NonAuthoritativeInformation = 203,
  NoContent = 204,
  ResetContent = 205,
  PartialContent = 206,
  MultipleChoices = 300,
  MovedPermanently = 301,
  Found = 302,
  SeeOther = 303,
  NotModified = 304,
  UseProxy = 305,
  Unused = 306,
  TemporaryRedirect = 307,
  BadRequest = 400,
  Unauthorized = 401,
  PaymentRequired = 402,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  NotAcceptable = 406,
  ProxyAuthenticationRequired = 407,
  RequestTimeout = 408,
  Conflict = 409,
  Gone = 410,
  LengthRequired = 411,
  PreconditionFailed = 412,
  RequestEntityTooLarge = 413,
  RequestURITooLarge = 414,
  UnsupportedMediaType = 415,
  RequestedRangeNotSatisfiable = 416,
  ExpectationFailed = 417,
  InternalServerError = 500,
  NotImplemented = 501,
  BadGateway = 502,
  ServiceUnavailable = 503,
  GatewayTimeout = 504,
  HttpVersionNotSupported = 505
};

std::map<HttpStatus, std::string> HttpStatusMap{
    {Continue, "Continue"},
    {SwitchingProtocols, "Switching Protocols"},
    {OK, "OK"},
    {Created, "Created"},
    {Accepted, "Accepted"},
    {NonAuthoritativeInformation, "NonAuthoritative Information"},
    {NoContent, "NoContent"},
    {ResetContent, "Reset Content"},
    {PartialContent, "Partial Content"},
    {MultipleChoices, "Multiple Choices"},
    {MovedPermanently, "Moved Permanently"},
    {Found, "Found"},
    {SeeOther, "See Other"},
    {NotModified, "Not Modified"},
    {UseProxy, "Use Proxy"},
    {Unused, "Unused"},
    {TemporaryRedirect, "Temporary Redirect"},
    {BadRequest, "Bad Request"},
    {Unauthorized, "Unauthorized"},
    {PaymentRequired, "Payment Required"},
    {Forbidden, "Forbidden"},
    {NotFound, "Not Found"},
    {MethodNotAllowed, "Method Not Allowed"},
    {NotAcceptable, "Not Acceptable"},
    {ProxyAuthenticationRequired, "Proxy Authentication Required"},
    {RequestTimeout, "Request Timeout"},
    {Conflict, "Conflict"},
    {Gone, "Gone"},
    {LengthRequired, "Length Required"},
    {PreconditionFailed, "Precondition Failed"},
    {RequestEntityTooLarge, "Request Entity Too Large"},
    {RequestURITooLarge, "Request URI Too Large"},
    {UnsupportedMediaType, "Unsupported Media Type"},
    {RequestedRangeNotSatisfiable, "Requested Range Not Satisfiable"},
    {ExpectationFailed, "Expectation Failed"},
    {InternalServerError, "Internal Server Error"},
    {NotImplemented, "Not Implemented"},
    {BadGateway, "Bad Gateway"},
    {ServiceUnavailable, "Service Unavailable"},
    {GatewayTimeout, "Gateway Timeout"},
    {HttpVersionNotSupported, "Http Version Not Supported"}};
}  // namespace webpp

#endif