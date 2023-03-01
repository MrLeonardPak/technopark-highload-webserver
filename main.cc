#include "server.hh"

#include <memory>

int main() {
  std::make_unique<server::Server>(80)->Start();
  return 0;
}
