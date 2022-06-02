#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>

namespace {
    std::mutex mtx{};
    constexpr std::size_t kIncrementTimes{ 1000 };
}

void atomic_f(std::atomic<int>& arg) {
    for (unsigned i{ 0 }; i < kIncrementTimes; ++i) {
        ++arg;
    }
}

void mutex_f(int& arg) {
    for (unsigned i{ 0 }; i < kIncrementTimes; ++i) {
        std::lock_guard tmp{ mtx };
        ++arg;
    }
}

int main() {

    std::atomic<int> a_i{};
    auto start = std::chrono::high_resolution_clock::now();
    std::thread a_first{ atomic_f, std::ref(a_i) };
    std::thread a_second{ atomic_f, std::ref(a_i) };
    a_first.join();
    a_second.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Atomic variant: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << "ns, a_i = " << a_i << "\n";

    int m_i{};
    start = std::chrono::high_resolution_clock::now();
    std::thread m_first{ mutex_f, std::ref(m_i) };
    std::thread m_second{ mutex_f, std::ref(m_i) };
    m_first.join();
    m_second.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Mutex variant: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << "ns, m_i = " << m_i << "\n";

    return 0;
}
