#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


std::mutex lock;
std::condition_variable cond1;
int ready = 0;

void producer() {
    while (true) {
        std::unique_lock<std::mutex> uniqueLock(lock);
        if (ready == 1) {
            uniqueLock.unlock();
            continue;
        }
        ready = 1;
        std::cout << "Поставщик: событие отправлено" << std::endl;
        cond1.notify_one();
        uniqueLock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> uniqueLock(lock);
        while(ready == 0)
        cond1.wait(uniqueLock, [] { return ready == 1; });
        ready = 0;
        std::cout << "Потребитель: событие получено" << std::endl;
        uniqueLock.unlock();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    consumerThread.join();

    return 0;
}