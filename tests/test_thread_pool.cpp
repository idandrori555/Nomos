#include "../include/thread_pool.hpp"
#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <future>
#include <vector>

TEST_CASE("ThreadPool - Basic Functionality", "[thread_pool]")
{
  using nomos::internal::ThreadPool;

  SECTION("Executes tasks successfully")
  {
    ThreadPool pool(2);
    std::promise<int> promise;
    auto future = promise.get_future();

    pool.enqueue([&promise]()
                 {
                   promise.set_value(42);
                 });

    REQUIRE(future.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
    CHECK(future.get() == 42);
  }

  SECTION("Executes multiple tasks concurrently")
  {
    ThreadPool pool(4);
    std::atomic<int> counter{0};
    const int num_tasks = 20;

    std::vector<std::promise<void>> promises(num_tasks);
    std::vector<std::future<void>> futures;
    futures.reserve(num_tasks);

    for (int i = 0; i < num_tasks; ++i)
    {
      futures.push_back(promises[i].get_future());
    }

    for (int i = 0; i < num_tasks; ++i)
    {
      pool.enqueue([&counter, &promises, i]()
                   {
                     counter.fetch_add(1, std::memory_order_relaxed);
                     promises[i].set_value();
                   });
    }

    for (auto &fut : futures)
    {
      REQUIRE(fut.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
    }

    CHECK(counter.load() == num_tasks);
  }
}

TEST_CASE("ThreadPool - Edge Cases and Destructor Lifecycle", "[thread_pool]")
{
  using nomos::internal::ThreadPool;

  SECTION("Pool cleans up properly with outstanding tasks on destruction")
  {
    std::atomic<int> completed_tasks{0};
    std::atomic<bool> block_threads{true};

    {
      ThreadPool pool(2);

      // Jam the active worker threads completely
      for (int i = 0; i < 2; ++i)
      {
        pool.enqueue([&block_threads]()
                     {
                       while (block_threads.load(std::memory_order_relaxed))
                       {
                         std::this_thread::yield();
                       }
                     });
      }

      // Enqueue extra tasks that will sit in the queue unpicked
      for (int i = 0; i < 5; ++i)
      {
        pool.enqueue([&completed_tasks]()
                     {
                       completed_tasks.fetch_add(1, std::memory_order_relaxed);
                     });
      }

      // Allow the active threads to exit their loops so they can see the stop flag
      block_threads.store(false);

      // Leaving this scope triggers ~ThreadPool() immediately.
    }

    SUCCEED("Destructor resolved cleanly without deadlocking.");
  }
}
