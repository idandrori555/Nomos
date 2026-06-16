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
  ~ThreadPool() noexcept;

  void enqueue(std::function<void()> task);

private:
  std::queue<std::function<void(void)>> m_tasks;
  std::mutex m_mutex;
  std::condition_variable_any m_cv;
  bool m_stop = false;
  std::vector<std::jthread> m_threads; // Destroyed first!
};
} // namespace nomos::internal
