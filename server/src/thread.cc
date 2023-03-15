#include "server/thread.hh"

#include "spdlog/spdlog.h"

#include "server/handler.hh"

namespace server {

ThreadPool::ThreadPool(unsigned int aMaxThread) {
  mThreads.reserve(aMaxThread);
  for (size_t i = 0; i < aMaxThread; ++i) {
    mThreads.emplace_back(&ThreadPool::ThreadMain, this);
  }
}

void ThreadPool::AddTask(int aSockerD) {
  auto lock = std::lock_guard(mMutex);
  mTaskQueue.emplace(aSockerD);
  spdlog::debug("Queue size {}", mTaskQueue.size());
  mCV.notify_one();
}

void ThreadPool::ThreadMain() {
  while (true) {
    int taskPayload;
    {
      auto lock = std::unique_lock(mMutex);

      mCV.wait(lock, [&]() { return !mTaskQueue.empty(); });
      taskPayload = mTaskQueue.front();
      mTaskQueue.pop();
    }
    Handler(taskPayload);
    spdlog::debug("Count {}", ++count);
  }
}

}  // namespace server
