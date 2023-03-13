#ifndef SERVER_I_TASK_MANAGER_HH
#define SERVER_I_TASK_MANAGER_HH

#include <functional>

namespace server {

class ITaskManager {
 public:
  using task_t = std::function<void()>;

  ITaskManager() = default;
  virtual ~ITaskManager() = default;

  virtual void AddTask(task_t) = 0;
};

}  // namespace server

#endif  // SERVER_I_TASK_MANAGER_HH
