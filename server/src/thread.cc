#include "server/thread.hh"

#include "spdlog/spdlog.h"

#include "server/handler.hh"

namespace server {

ThreadPool::ThreadPool(unsigned int aMaxThread) {
  if (std::thread::hardware_concurrency() < aMaxThread) {
    aMaxThread = std::thread::hardware_concurrency();
    spdlog::warn(
        "Max number of threads in current system is {}. {} threads are "
        "forcibly set.",
        aMaxThread, aMaxThread);
  };
  mThreads.reserve(aMaxThread);
  for (size_t i = 0; i < aMaxThread; ++i) {
    mThreads.emplace_back(&ThreadPool::ThreadMain, this);
  }
}

void ThreadPool::AddTask(int aSockerD,
                         std::array<char, MAX_RECV_LEN> const& aBuffer) {
  auto lock = std::lock_guard(mMutex);
  mTaskQueue.emplace(aSockerD, aBuffer);
  mCV.notify_one();
}

void ThreadPool::ThreadMain() {
  int i = 0;
  while (true) {
    auto lock = std::unique_lock(mMutex);

    mCV.wait(lock, [&]() { return !mTaskQueue.empty(); });
    Handler(mTaskQueue.front().first, mTaskQueue.front().second);
    mTaskQueue.pop();

    std::pair<int, buffet_t> taskPayload;
    {
      auto lock = std::unique_lock(mMutex);

      mCV.wait(lock, [&]() { return !mTaskQueue.empty(); });
      taskPayload = mTaskQueue.front();
      mTaskQueue.pop();
    }
#ifndef Debug
    pthread_yield();
#endif  // Debug
    Handler(taskPayload.first, taskPayload.second);
    spdlog::debug("Task {}", mTaskQueue.size());
  }
}

}  // namespace server
