#ifndef SERVER_HANDLER_HH
#define SERVER_HANDLER_HH

#include <array>

#include "server/server.hh"

namespace server {

void Handler(int aSocket, buffet_t const& aBuffer);

}  // namespace server

#endif  // SERVER_HANDLER_HH
