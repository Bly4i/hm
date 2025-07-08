#include "thread_pool.h"

void task1() {
    std::cout << "Executing task1 from thread: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void task2() {
    std::cout << "Executing task2 from thread: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
}

int main() {
    size_t num_threads = std::thread::hardware_concurrency();
    thread_pool pool(num_threads);

    for (int i = 0; i < 5; ++i) {
        std::future<void> future1 = pool.submit(task1);
        std::future<void> future2 = pool.submit(task2);

        future1.get();
        future2.get();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
