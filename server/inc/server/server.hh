#ifndef SERVER_SERVER_HH
#define SERVER_SERVER_HH

#include <sstream>

namespace server {

class Server {
 public:
  Server(int aPort);
  void Start();

  size_t constexpr static sMaxRecvLen = 1024;
  static void Process(std::istream& in, std::ostream& out);

 private:
  int const kPort;

  int mSocketFD;
  int mSocket;
};

}  // namespace server

#endif  // SERVER_SERVER_HH
