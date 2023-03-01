#include "server.hh"

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

namespace server {

void Hello() {
  std::cout << "Hello" << std::endl;
}

Server::Server(int aPort) : kPort(aPort) {}

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

  char buffer[sMaxRecvLen];
  while (true) {
    if ((mSocket = accept(mSocketFD, reinterpret_cast<sockaddr*>(&address),
                          &addrlen)) < 0) {
      throw std::runtime_error("Accept failed");
    }

    auto result = read(mSocket, buffer, sMaxRecvLen);
    switch (result) {
      case -1:
        throw std::runtime_error("Read failed");
        break;
      case 0:
        throw std::runtime_error("Connection closed");
        break;
      default:
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
  while (in) {
    std::string s;
    std::getline(in, s);
    std::cout << s << std::endl;
  }

  out << "HTTP/1.1 200 OK\r\n"
      << "Version: HTTP/1.1\r\n"
      << "Content-Type: text/html; charset=utf-8\r\n"
      << "Content-Length: " << 0 << "\r\n\r\n";
}

}  // namespace server
