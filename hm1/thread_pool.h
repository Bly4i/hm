#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <chrono>
#include <atomic>
#include <stdexcept>

template <typename T>
class safe_queue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_{ false };

public:
    safe_queue() = default;
    safe_queue(const safe_queue&) = delete;
    safe_queue& operator=(const safe_queue&) = delete;

    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        condition_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !queue_.empty() || stop_.load(std::memory_order_relaxed); });
        if (queue_.empty()) {
            throw std::runtime_error("Queue is empty, stopping thread.");
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    void stop() {
        stop_.store(true, std::memory_order_relaxed);
        condition_.notify_all();
    }
};

class thread_pool {
private:
    std::vector<std::thread> threads_;
    safe_queue<std::function<void()>> tasks_;
    std::atomic<bool> stop_{ false };
    std::mutex mutex_;
    std::condition_variable condition_;

    void work(); 

public:
    thread_pool(size_t num_threads);
    ~thread_pool();

    template <typename F, typename... Args>
    std::future<typename std::result_of<F(Args...)>::type> submit(F&& f, Args&&... args);
};

template <typename F, typename... Args>
std::future<typename std::result_of<F(Args...)>::type> thread_pool::submit(F&& f, Args&&... args) {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    tasks_.push([task]() { (*task)(); });
    return res;
}


#endif 
