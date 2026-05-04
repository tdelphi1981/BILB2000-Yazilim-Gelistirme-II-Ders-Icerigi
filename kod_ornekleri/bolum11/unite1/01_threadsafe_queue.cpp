/**
 * Thread-Safe Queue - Şablon Sınıf Tasarımı
 *
 * Bölüm 11 Ünite 1 Konu 1
 *
 * Bu örnekte:
 *  - Şablon ThreadSafeQueue<T> sınıfını yazıyoruz
 *  - tryPop, waitAndPop, kapat metotlarını içerir
 *  - Üretici-tüketici örneği ile test ediyoruz
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 01_tsqueue 01_threadsafe_queue.cpp
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(T deger) {
        {
            std::lock_guard<std::mutex> kilit(m_);
            if (kapali_) {
                throw std::runtime_error("Kuyruk kapali");
            }
            kuyruk_.push(std::move(deger));
        }
        cv_.notify_one();
    }

    std::optional<T> tryPop() {
        std::lock_guard<std::mutex> kilit(m_);
        if (kuyruk_.empty()) return std::nullopt;
        T d = std::move(kuyruk_.front());
        kuyruk_.pop();
        return d;
    }

    bool waitAndPop(T& cikti) {
        std::unique_lock<std::mutex> kilit(m_);
        cv_.wait(kilit, [this] {
            return !kuyruk_.empty() || kapali_;
        });
        if (kuyruk_.empty()) return false;
        cikti = std::move(kuyruk_.front());
        kuyruk_.pop();
        return true;
    }

    void kapat() {
        {
            std::lock_guard<std::mutex> kilit(m_);
            kapali_ = true;
        }
        cv_.notify_all();
    }

    std::size_t boyut() const {
        std::lock_guard<std::mutex> kilit(m_);
        return kuyruk_.size();
    }

    bool kapaliMi() const {
        std::lock_guard<std::mutex> kilit(m_);
        return kapali_;
    }

private:
    mutable std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> kuyruk_;
    bool kapali_ = false;
};

int main() {
    ThreadSafeQueue<int> kuyruk;

    // Üretici thread
    std::thread uretici([&] {
        for (int i = 1; i <= 20; ++i) {
            kuyruk.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        kuyruk.kapat();
    });

    // 3 tüketici
    std::vector<std::thread> tuketiciler;
    std::mutex coutMutex;
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

/*
BEKLENEN CIKTI:
---------------
Tuketici 1: 1
Tuketici 2: 2
Tuketici 3: 3
... (rastgele dağılımla 20 satır)
Kuyruk bos. Kapali: 1
*/
