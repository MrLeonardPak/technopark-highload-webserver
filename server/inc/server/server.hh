#ifndef SERVER_SERVER_HH
#define SERVER_SERVER_HH

#include <array>
#include <memory>
#include <sstream>

#include "server/config.hh"

namespace server {

size_t constexpr MAX_RECV_LEN = 1024;

using buffet_t = std::array<char, MAX_RECV_LEN>;

class Server {
 public:
  Server(int aPort);
  void Start();

  static void Process(std::istream& in, int aSocket);

 private:
  int const kPort;

  int mSocketFD;
  // int socket;
};

}  // namespace server

#endif  // SERVER_SERVER_HH
