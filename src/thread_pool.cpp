#include "thread_pool.hpp"
#include <stop_token>

namespace nomos::internal
{
ThreadPool::ThreadPool(size_t num_threads)
{
  m_threads.reserve(num_threads);
  for (auto i{0zu}; i < num_threads; i++)
  {
    m_threads.emplace_back([this](std::stop_token stop_token)
                           {
                             while (!stop_token.stop_requested())
                             {
                               std::function<void()> task;
                               {
                                 std::unique_lock lock{m_mutex};
                                 m_cv.wait(lock, [this, &stop_token]
                                           { // Capture token by reference
                                             return m_stop || !m_tasks.empty() || stop_token.stop_requested();
                                           });

                                 if ((m_stop || stop_token.stop_requested()) && m_tasks.empty())
                                   return;

                                 if (m_tasks.empty())
                                   continue;

                                 task = std::move(m_tasks.front());
                                 m_tasks.pop();
                               }

                               if (task)
                               {
                                 task();
                               }
                             }
                           });
  }
}

ThreadPool::~ThreadPool() noexcept
{
  {
    std::unique_lock lock{m_mutex};
    m_stop = true;
  }

  m_cv.notify_all();
}

void ThreadPool::enqueue(std::function<void()> task)
{
  {
    std::unique_lock lock{m_mutex};

    if (m_stop)
      return;

    m_tasks.push(std::move(task));
  }
  m_cv.notify_one(); // Notify the thread that there is a task available to execute
}

}; // namespace nomos::internal
