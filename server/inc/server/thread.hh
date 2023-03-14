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

  void AddTask(int aSockerD);

 private:
  std::vector<std::thread> mThreads;
  std::queue<int> mTaskQueue;
  std::mutex mMutex;
  std::condition_variable mCV;
  size_t count = 0;

  void ThreadMain();
};

}  // namespace server

#endif  // SERVER_THREAD_HH
