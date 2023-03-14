#include <signal.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "server/config.hh"
#include "server/server.hh"
#include "server/thread.hh"

int constexpr RESTART_TIME_SEC = 3;

int main() {
  using namespace std::chrono_literals;
  signal(SIGPIPE, SIG_IGN);
  while (1) {
    try {
      auto pool = std::make_unique<server::ThreadPool>(
          server::Config<int>("thread_limit"));
      std::make_unique<server::Server>(80, std::move(pool))->Start();
    } catch (std::exception const& e) {
      std::cout << e.what() << std::endl;
    }
    for (int i = 0; i < RESTART_TIME_SEC; ++i) {
      std::cout << "Enable server via " << RESTART_TIME_SEC - i << " sec!"
                << std::endl;
      std::this_thread::sleep_for(1s);
    }
  }

  return 0;
}
