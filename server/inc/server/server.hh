#ifndef SERVER_SERVER_HH
#define SERVER_SERVER_HH

#include <sstream>

#include "server/config.hh"

namespace server {

size_t constexpr MAX_RECV_LEN = 1024;

class Server {
 public:
  Server(int aPort, std::string const& aConfigPath);
  void Start();

  static void Process(std::istream& in, std::ostream& out);

 private:
  int const kPort;

  int mSocketFD;
  int mSocket;
};

}  // namespace server

#endif  // SERVER_SERVER_HH
