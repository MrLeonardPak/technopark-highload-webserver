#include "server/http.hh"

#include <chrono>
#include <iomanip>

#include "spdlog/spdlog.h"

#include "server/config.hh"

namespace server {

std::string HTTP_GEN_DATE_HEADER() noexcept {
  auto date =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  auto tm = std::gmtime(&date);
  std::stringstream ss;
  ss << "Date: "
     << std::put_time(std::gmtime(&date), "%a, %d %b %Y %H:%M:%S %Z") << "\r\n";
  return ss.str();
}

std::string HTTP_GEN_SERVER_HEADER() noexcept {
  return "Server: C++ with thread pool\r\n";
}
std::string HTTP_GEN_CONNECTION_HEADER() noexcept {
  return "Connection: close\r\n";
}

std::string HTTP_GEN_CONTENT_TYPE(std::string const& type) noexcept {
  try {
    return "Content-Type: " + MIME.at(type) + "\r\n";
  } catch (std::out_of_range) {
    return "";
  }
}

std::string HTTP_GEN_CONTENT_LENGTH(int len) noexcept {
  std::stringstream ss;
  ss << "Content-Length: " << len << "\r\n";
  return ss.str();
}

bool IsSubpath(std::filesystem::path const& path,
               std::filesystem::path const& base) {
  auto rel = std::filesystem::relative(path, base);
  return !rel.empty() && rel.native()[0] != '.';
}

char HexToChar(char a, char b) {
  std::stringstream ss;
  ss << std::hex << a << b;
  int n;
  ss >> n;
  return n;
}

std::string PercentDecode(std::string_view str) {
  std::string res = "";
  for (std::string_view::iterator it = str.begin(); it != str.end(); ++it) {
    if (*it == '%') {
      res += HexToChar(it[1], it[2]);
      it += 2;
    } else {
      res += *it;
    }
  }
  return res;
}

Request::Method ParseMethod(std::istream& in) {
  std::string method;
  in >> method;
  spdlog::debug("Method: {}", method);
  if (method == "HEAD") {
    return Request::HEAD;
  } else if (method == "GET") {
    return Request::GET;
  } else {
    throw 405;
  }
}

std::filesystem::path ParseLocation(std::istream& in) {
  std::string location;
  in >> location;
  location = PercentDecode(location);
  location = location.substr(0, location.find('?'));

  auto path = std::filesystem::weakly_canonical(Config(ROOT_CONF) + location);
  if (!IsSubpath(path, Config(ROOT_CONF))) {
    spdlog::debug("Out of root: {}", path.native());
    throw 403;
  }
  if (path.extension().native().empty()) {
    path += "/index.html";
  }
  try {
    return std::filesystem::canonical(path);
  } catch (std::exception) {
    spdlog::debug("No file: {}", path.native());
    throw 404;
  }
}

Request::Request(std::istream& in)
    : mMethod(ParseMethod(in)), mLocation(ParseLocation(in)) {
  spdlog::debug("----------REQUEST----------");
  spdlog::debug("Method: {}", mMethod);
  spdlog::debug("Location: {}", mLocation.native());
  spdlog::debug("---------------------------");
}

}  // namespace server
