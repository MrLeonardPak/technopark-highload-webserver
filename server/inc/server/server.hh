#ifndef SERVER_SERVER_HH
#define SERVER_SERVER_HH

#include <memory>
#include <sstream>

#include "server/config.hh"
#include "server/interface/i-task-manager.hh"

namespace server {

size_t constexpr MAX_RECV_LEN = 1024;

class Server {
 public:
  Server(int aPort, std::unique_ptr<ITaskManager>);
  void Start();

  static void Process(std::istream& in, int aSocket);

 private:
  int const kPort;
  std::unique_ptr<ITaskManager> mTaskMenager;

  int mSocketFD;
  // int socket;
};

}  // namespace server

#endif  // SERVER_SERVER_HH
