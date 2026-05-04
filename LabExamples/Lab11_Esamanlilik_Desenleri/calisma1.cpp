/**
 * Lab 11 - Çalışma 1: ThreadSafeQueue
 *
 * Bu dosyadaki GOREV yorumlarını uygun kodla değiştirin.
 *
 * Derleme: g++ -std=c++20 -pthread -O2 -o calisma1 calisma1.cpp
 * Çalıştırma: ./calisma1
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(T deger) {
        // GOREV 1: Mutex korumali kuyruga ekleme yap
        //   - std::lock_guard ile m_'i kilitle (ic kapsamda)
        //   - kuyruk_.push(std::move(deger))
        //   - Kilit serbest kaldiktan SONRA cv_.notify_one() cagir

    }

    std::optional<T> tryPop() {
        // GOREV 2: Bossa nullopt, doluysa move ile cikar
        //   - std::lock_guard kilit
        //   - if (kuyruk_.empty()) return std::nullopt;
        //   - T x = std::move(kuyruk_.front()); kuyruk_.pop(); return x;

        return std::nullopt;
    }

    bool waitAndPop(T& cikti) {
        // GOREV 3: Predicate ile wait; bossa false donder
        //   - std::unique_lock kilit (m_)
        //   - cv_.wait(kilit, [this] { return !kuyruk_.empty() || kapali_; });
        //   - if (kuyruk_.empty()) return false;  // Kapali ve bos
        //   - cikti = std::move(kuyruk_.front()); kuyruk_.pop(); return true;

        return false;
    }

    void kapat() {
        // GOREV 4: kapali_ bayragini true yap; notify_all cagir
        //   - { std::lock_guard g(m_); kapali_ = true; }
        //   - cv_.notify_all();

    }

    bool kapaliMi() const {
        std::lock_guard<std::mutex> g(m_);
        return kapali_;
    }

private:
    mutable std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> kuyruk_;
    bool kapali_ = false;
};

// =============================================================================
// TEST: 1 üretici 100 sayı atar, 3 tüketici çeker
// =============================================================================
int main() {
    ThreadSafeQueue<int> kuyruk;
    std::mutex coutMutex;

    std::thread uretici([&] {
        for (int i = 1; i <= 100; ++i) {
            kuyruk.push(i);
        }
        kuyruk.kapat();
    });

    std::vector<std::thread> tuketiciler;
    for (int t = 1; t <= 3; ++t) {
        tuketiciler.emplace_back([&, t] {
            int x;
            while (kuyruk.waitAndPop(x)) {
                std::lock_guard<std::mutex> g(coutMutex);
                std::cout << "Tuketici " << t << ": " << x << '\n';
            }
        });
    }

    uretici.join();
    for (auto& t : tuketiciler) t.join();

    std::cout << "\nKuyruk bos. Kapali: " << kuyruk.kapaliMi() << '\n';
    return 0;
}
