#include "thread_pool.hpp"

namespace renderer {

size_t ThreadPool::k_threads = std::thread::hardware_concurrency();

ThreadPool& ThreadPool::Get() {
    static ThreadPool instance;
    return instance;
}

void ThreadPool::SetThreadsCount(const size_t num_threads) {
    if (num_threads == 0) {
        k_threads = std::thread::hardware_concurrency();
    } else {
        k_threads = num_threads;
    }
}

size_t ThreadPool::GetThreadsCount() {
    return k_threads;
}

void ThreadPool::Enqueue(std::function<void()>&& task) {
    {
        std::unique_lock lock{mutex_};
        tasks_.push(task);
    }
    cv_task_.notify_one();
}

void ThreadPool::WaitAll() {
    std::unique_lock lock{mutex_};
    cv_done_.wait(lock, [this]() { return tasks_.empty() and active_tasks_ == 0; });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock{mutex_};
        stop_ = true;
    }
    cv_task_.notify_all();
    for (auto& thread : workers_) {
        thread.join();
    }
}

ThreadPool::ThreadPool() {
    for (size_t i = 0; i < k_threads; ++i) {
        workers_.emplace_back([this]() { WorkerLoop(); });
    }
}

void ThreadPool::WorkerLoop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock{mutex_};
            cv_task_.wait(lock, [this]() { return stop_ or (not tasks_.empty()); });

            if (stop_ and tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
            ++active_tasks_;
        }
        task();
        {
            std::unique_lock lock{mutex_};
            --active_tasks_;
            if (tasks_.empty() and (active_tasks_ == 0)) {
                cv_done_.notify_all();
            }
        }
    }
}

}  // namespace renderer
