#include "../include/thread_pool.hpp"
#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <future>

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

    // Block until the worker thread executes the task
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

    // Wait for all asynchronous tasks to complete
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
    {
      ThreadPool pool(2);

      // Push some long-running tasks to jam the workers
      for (int i = 0; i < 2; ++i)
      {
        pool.enqueue([]()
                     {
                       std::this_thread::sleep_for(std::chrono::milliseconds(50));
                     });
      }

      // Enqueue additional queued jobs that haven't been picked up yet
      for (int i = 0; i < 5; ++i)
      {
        pool.enqueue([&completed_tasks]()
                     {
                       completed_tasks.fetch_add(1, std::memory_order_relaxed);
                     });
      }

      // Leaving scope triggers ~ThreadPool() immediately.
      // The workers will drain the existing running ones and check m_stop.
    }

    // Verify the code cleanly stops without causing any hang or deadlock.
    SUCCEED("Destructor resolved cleanly.");
  }
}
