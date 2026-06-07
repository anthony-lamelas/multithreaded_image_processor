#include "../include/thread_pool.hpp"

ThreadPool::ThreadPool(std::size_t num_threads) : stop(false) {
    for (std::size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                // Access the queue
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);

                    // Sleep until there's a task or the pool is stopped
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });

                    // If woken up to stop and the queue is empty, exit the thread
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    // Grab the task and remove it from the queue
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                } // Lock is automatically released here

                // Run the task outside the lock so other threads can still grab tasks
                task();
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::move(task));
    } // Lock released

    // Wake up one sleeping thread to handle the new task
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    } // Lock released

    // Wake up all threads so they can check the stop flag and exit
    condition.notify_all();

    // Wait for all threads to finish
    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
