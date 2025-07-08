#include "thread_pool.h"

thread_pool::thread_pool(size_t num_threads) : threads_(num_threads) {
    for (size_t i = 0; i < num_threads; ++i) {
        threads_[i] = std::thread([this] { work(); });
    }
}

thread_pool::~thread_pool() {
    stop_.store(true, std::memory_order_relaxed);
    tasks_.stop(); 
    condition_.notify_all(); 

    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void thread_pool::work() {
    while (!stop_.load(std::memory_order_relaxed)) {
        try {
            std::function<void()> task = tasks_.pop();
            if (stop_.load(std::memory_order_relaxed) && !task) {
                return;
            }
            task();

        }
        catch (const std::exception& e) {
            std::cerr << "Exception in thread: " << e.what() << std::endl;
            break; 
        }
    }
}
