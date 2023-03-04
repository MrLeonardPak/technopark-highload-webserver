#include "server/server.hh"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

int constexpr RESTART_TIME_SEC = 3;

int main() {
  using namespace std::chrono_literals;
  while (1) {
    try {
      std::make_unique<server::Server>(80, "")->Start();
    } catch (std::exception const& e) {
      std::clog << e.what() << std::endl;
    }
    for (int i = 0; i < RESTART_TIME_SEC; ++i) {
      std::clog << "Enable server via " << RESTART_TIME_SEC - i << " sec!"
                << std::endl;
      std::this_thread::sleep_for(1s);
    }
  }

  return 0;
}
