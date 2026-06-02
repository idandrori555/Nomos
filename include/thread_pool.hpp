#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace nomos::internal
{
class ThreadPool
{
public:
  ThreadPool(size_t num_threads = std::thread::hardware_concurrency());
  ~ThreadPool() = default; // becuase we use jthreads, we don't need to manually join.

  void enqueue(std::function<void()> task);

private:
  std::vector<std::jthread> m_threads;
  std::queue<std::function<void(void)>> m_tasks;
  std::mutex m_mutex;
  std::condition_variable_any m_cv;
  bool m_stop = false;
};
} // namespace nomos::internal
