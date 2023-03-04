#ifndef SERVER_HTTP_HH
#define SERVER_HTTP_HH

#include <filesystem>
#include <sstream>
#include <unordered_map>

namespace server {

std::unordered_map<std::string, std::string> const MIME = {
    {".html", "text/html"},     {".css", "text/css"},
    {".js", "text/javascript"}, {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},    {".png", "image/png"},
    {".gif", "image/gif"},      {".swf", "application/x-shockwave-flash"}};
std::string_view constexpr ROOT_CONF = "document_root";

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

 private:
  Method mMethod;
  std::filesystem::path mLocation;
};

class Response {
 public:
  Response(Request const& req);
  ~Response() = default;
};

}  // namespace server

#endif  // SERVER_HTTP_HH
