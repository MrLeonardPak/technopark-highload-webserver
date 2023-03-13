#ifndef SERVER_HTTP_HH
#define SERVER_HTTP_HH

#include <filesystem>
#include <sstream>
#include <string>
#include <unordered_map>

namespace server {

std::unordered_map<std::string, std::string> const MIME = {
    {".html", "text/html"},     {".css", "text/css"},
    {".js", "text/javascript"}, {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},    {".png", "image/png"},
    {".gif", "image/gif"},      {".swf", "application/x-shockwave-flash"}};

std::unordered_map<int, std::string> const STATUS = {
    {200, "200 OK"},
    {403, "403 Forbidden"},
    {404, "404 Not Found"},
    {405, "405 Method Not Allowed"},
    {500, "500 Internal Server Error"},
};
std::string const ROOT_CONF = "document_root";
std::string const HTTP_VERSION = "HTTP/1.1";
std::string const CRLF = "\r\n";

std::string HTTP_GEN_DATE_HEADER() noexcept;
std::string HTTP_GEN_SERVER_HEADER() noexcept;
std::string HTTP_GEN_CONNECTION_HEADER() noexcept;
std::string HTTP_GEN_CONTENT_TYPE(std::string const& type) noexcept;
std::string HTTP_GEN_CONTENT_LENGTH(int len) noexcept;

class Request {
 public:
  enum Method { HEAD, GET };

  Request(std::istream& in);
  ~Request() = default;

  Method getMethod() const;
  std::filesystem::path getLocation() const;

 private:
  Method mMethod;
  std::filesystem::path mLocation;
};

class Response {
 public:
  Response(Request const& aRequest);
  ~Response() = default;

  std::string getInfo() const;

  static std::string BuildErrorResponse(int aStatusCode);

 private:
  Request mRequest;
  std::string mStatusLine = HTTP_VERSION + " " + STATUS.at(200) + CRLF;
  std::string mHeaders = HTTP_GEN_SERVER_HEADER() + HTTP_GEN_DATE_HEADER() +
                         HTTP_GEN_CONNECTION_HEADER();
};

}  // namespace server

#endif  // SERVER_HTTP_HH
