#include "server/handler.hh"

#include <sys/socket.h>

#include "spdlog/spdlog.h"

#include "server/file.hh"
#include "server/http.hh"

namespace server {

void Handler(int aSocket, buffet_t const& aBuffer) {
  std::stringstream in{aBuffer.cbegin()};
  try {
    auto request = Request(in);
    auto responseRaw = Response(request).getInfo();
    if (auto err = send(aSocket, responseRaw.data(), responseRaw.length(),
                        MSG_NOSIGNAL);
        err == -1) {
      if (errno == EPIPE) {
        spdlog::warn("EPIPE send");
        goto close_socket;
      }
      throw std::runtime_error("send error: " + std::to_string(errno));
    }

    if (request.getMethod() == Request::GET) {
      FileSend(aSocket, request.getLocation());
    }

  } catch (int errorCode) {
    auto errorResponse = Response::BuildErrorResponse(errorCode);
    send(aSocket, errorResponse.data(), errorResponse.length(), MSG_NOSIGNAL);
    if (auto err = send(aSocket, errorResponse.data(), errorResponse.length(),
                        MSG_NOSIGNAL);
        err == -1) {
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
