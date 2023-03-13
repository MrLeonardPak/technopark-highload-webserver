#include "server/server.hh"

#include <netinet/in.h>
#include <sys/socket.h>
#include <array>

#include "spdlog/spdlog.h"

#include "server/config.hh"
#include "server/file.hh"
#include "server/http.hh"

namespace server {

Server::Server(int aPort, std::unique_ptr<ITaskManager> aTaskManager)
    : kPort(aPort), mTaskMenager(std::move(aTaskManager)) {
  spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [thread %t] %v");
#ifdef DEBUG
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("DEBUG MODE");
#endif
  spdlog::debug("Path: {}", Config<std::string>("document_root"));
  spdlog::debug("Threads: {}", Config<int>("thread_limit"));
}

void Server::Start() {
  if ((mSocketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    throw std::runtime_error("Socket failed");
  }

  auto constexpr opt = 1;
  if (setsockopt(mSocketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    throw std::runtime_error("Setsockopt failed");
  }

  auto address = sockaddr_in();
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(kPort);
  socklen_t addrlen = sizeof(address);

  if (bind(mSocketFD, reinterpret_cast<sockaddr*>(&address), addrlen) < 0) {
    throw std::runtime_error("Bind failed");
  }
  if (listen(mSocketFD, 3) < 0) {
    throw std::runtime_error("Listen failed");
  }

  int socketD;
  std::array<char, MAX_RECV_LEN> buffer;
  while (true) {
    if ((socketD = accept(mSocketFD, reinterpret_cast<sockaddr*>(&address),
                          &addrlen)) < 0) {
      throw std::runtime_error("Accept failed");
    }

    auto result = read(socketD, buffer.data(), MAX_RECV_LEN);
    switch (result) {
      case -1:
        throw std::runtime_error("Read failed");
        break;
      case 0:
        throw std::runtime_error("Connection closed");
        break;
      default:
        mTaskMenager->AddTask([socketD, buffer]() {
          std::stringstream in{buffer.cbegin()};
          try {
            auto request = Request(in);
            auto response = Response(request);
            send(socketD, response.getInfo().data(),
                 response.getInfo().length(), 0);
            if (request.getMethod() == Request::GET) {
              FileSend(socketD, request.getLocation());
            }
          } catch (int errorCode) {
            auto errorResponse = Response::BuildErrorResponse(errorCode);
            send(socketD, errorResponse.data(), errorResponse.length(), 0);
          }
          close(socketD);
        });
        break;
    }
  }

  shutdown(mSocketFD, SHUT_RDWR);
}

}  // namespace server
