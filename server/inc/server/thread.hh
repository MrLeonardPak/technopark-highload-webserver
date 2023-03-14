#ifndef SERVER_THREAD_HH
#define SERVER_THREAD_HH

#include <array>
#include <condition_variable>
#include <queue>
#include <thread>

#include "server/server.hh"

namespace server {
class ThreadPool {
 public:
  ThreadPool(unsigned int aMaxThread);
  ~ThreadPool() = default;

  void AddTask(int aSockerD, std::array<char, MAX_RECV_LEN> const& aBuffer);

 private:
  std::vector<std::jthread> mThreads;
  std::queue<std::pair<int, std::array<char, MAX_RECV_LEN>>> mTaskQueue;
  std::mutex mMutex;
  std::condition_variable mCV;
  // std::atomic_flag mLock;

  void ThreadMain();
};

}  // namespace server

#endif  // SERVER_THREAD_HH
