#include "server/server.hh"

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "spdlog/spdlog.h"

#include "server/config.hh"
#include "server/http.hh"

namespace server {

Server::Server(int aPort, std::string const& aConfigPath) : kPort(aPort) {
  spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [thread %t] %v");
#ifdef DEBUG
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("DEBUG MODE");
#endif
  spdlog::debug("Path: {}", Config("document_root"));
  spdlog::debug("Threads: {}", std::stoi(Config("thread_limit")));
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

  char buffer[MAX_RECV_LEN];
  while (true) {
    if ((mSocket = accept(mSocketFD, reinterpret_cast<sockaddr*>(&address),
                          &addrlen)) < 0) {
      throw std::runtime_error("Accept failed");
    }

    auto result = read(mSocket, buffer, MAX_RECV_LEN);
    switch (result) {
      case -1:
        throw std::runtime_error("Read failed");
        break;
      case 0:
        throw std::runtime_error("Connection closed");
        break;
      default:
        spdlog::debug("Raw request:\n{}", buffer);
        std::stringstream out;
        std::stringstream in{buffer};
        Process(in, out);
        send(mSocket, out.str().c_str(), out.str().length(), 0);
        break;
    }

    close(mSocket);
  }

  shutdown(mSocketFD, SHUT_RDWR);
}

void Server::Process(std::istream& in, std::ostream& out) {
  out << "HTTP/1.1 ";
  try {
    auto request = Request(in);
    out << "200\r\n";
  } catch (int code) {
    out << code << "\r\n";
  }

  auto date =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  auto tm = std::gmtime(&date);
  out << HTTP_GEN_SERVER_HEADER() << HTTP_GEN_DATE_HEADER()
      << HTTP_GEN_CONNECTION_HEADER() << "\r\n";
}

}  // namespace server
