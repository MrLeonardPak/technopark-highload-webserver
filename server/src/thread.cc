#include "server/thread.hh"

#include <spdlog/spdlog.h>

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

void ThreadPool::AddTask(task_t aTask) {
  auto lock = std::lock_guard(mMutex);
  mTaskQueue.push(aTask);
  mCV.notify_one();
}

void ThreadPool::ThreadMain() {
  int i = 0;
  while (true) {
    auto lock = std::unique_lock(mMutex);
    while (mTaskQueue.empty()) {
      mCV.wait(lock);
    }
    mTaskQueue.front()();
    mTaskQueue.pop();
  }
}

}  // namespace server
