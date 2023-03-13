#ifndef SERVER_THREAD_HH
#define SERVER_THREAD_HH

#include <condition_variable>
#include <queue>
#include <thread>

#include "server/interface/i-task-manager.hh"

namespace server {
class ThreadPool : public ITaskManager {
 public:
  ThreadPool(uint aMaxThread);
  ~ThreadPool() = default;

  void AddTask(task_t aTask) override;

 private:
  std::vector<std::jthread> mThreads;
  std::queue<task_t> mTaskQueue;
  std::mutex mMutex;
  std::condition_variable mCV;
  bool mPingPong = false;
  // std::atomic_flag mLock;

  void ThreadMain();
};

}  // namespace server

#endif  // SERVER_THREAD_HH
