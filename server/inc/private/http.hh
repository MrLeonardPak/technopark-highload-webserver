#ifndef SERVER_HTTP_HH
#define SERVER_HTTP_HH

#include <sstream>

namespace server {

class Request {
  enum Type { GET };

 public:
  Request(std::istream& in);

 private:
  Type type;
  std::string location;
};

class Response {
  /* data */
};

}  // namespace server

#endif  // SERVER_HTTP_HH
