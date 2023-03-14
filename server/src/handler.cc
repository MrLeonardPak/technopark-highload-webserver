#include "server/handler.hh"

#include <sys/socket.h>

#include "spdlog/spdlog.h"

#include "server/file.hh"
#include "server/http.hh"

namespace server {

void Handler(int aSocket) {
  std::array<char, MAX_RECV_LEN> buffer;
  try {
    auto result = read(aSocket, buffer.data(), MAX_RECV_LEN);
    switch (result) {
      case -1:
        throw std::runtime_error("Read failed");
      case 0:
        goto close_socket;
      default: {
        std::stringstream in{buffer.cbegin()};
        auto request = Request(in);
        auto responseRaw = Response(request).getInfo();
        if (send(aSocket, responseRaw.data(), responseRaw.length(),
                 MSG_NOSIGNAL) == -1) {
          if (errno == EPIPE) {
            spdlog::warn("EPIPE send");
            goto close_socket;
          }
          throw std::runtime_error("send error: " + std::to_string(errno));
        }

        if (request.getMethod() == Request::GET) {
          FileSend(aSocket, request.getLocation());
        }
        break;
      }
    }

  } catch (int errorCode) {
    auto errorResponse = Response::BuildErrorResponse(errorCode);
    if (send(aSocket, errorResponse.data(), errorResponse.length(),
             MSG_NOSIGNAL) == -1) {
      if (errno == EPIPE) {
        spdlog::warn("EPIPE send error");
        goto close_socket;
      }
      throw std::runtime_error("send error: " + std::to_string(errno));
    }
  }
close_socket:
  close(aSocket);
}

}  // namespace server
