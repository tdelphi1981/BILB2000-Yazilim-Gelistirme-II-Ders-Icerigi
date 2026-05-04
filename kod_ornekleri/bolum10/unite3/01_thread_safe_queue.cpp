/**
 * Thread-Safe Queue Şablonu
 *
 * Bu program, tip bağımsız, thread-safe bir kuyruk sınıfının
 * tam uygulamasını ve çoklu tüketiciyle test örneğini gösterir.
 *
 * Öğrenilecek kavramlar:
 * - Şablon (template) sınıf tasarımı
 * - mutex + condition_variable ile güvenli kuyruk
 * - wait_and_pop ve try_pop arasındaki fark
 * - std::optional<T> dönüş tipi
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 3 - Thread Havuzu Tasarımı
 *
 * Derleme: g++ -std=c++20 -pthread -o 01_queue 01_thread_safe_queue.cpp
 * Çalıştırma: ./01_queue
 */

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <thread>
#include <vector>
#include <iostream>
#include <string>

template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(T deger) {
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            if (kapaniyor_) return;
            kuyruk_.push(std::move(deger));
        }
        cv_.notify_one();
    }

    bool wait_and_pop(T& deger) {
        std::unique_lock<std::mutex> kilit(mtx_);
        cv_.wait(kilit, [this] {
            return !kuyruk_.empty() || kapaniyor_;
        });
        if (kuyruk_.empty()) return false;
        deger = std::move(kuyruk_.front());
        kuyruk_.pop();
        return true;
    }

    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> kilit(mtx_);
        if (kuyruk_.empty()) return std::nullopt;
        T deger = std::move(kuyruk_.front());
        kuyruk_.pop();
        return deger;
    }

    void close() {
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            kapaniyor_ = true;
        }
        cv_.notify_all();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> kilit(mtx_);
        return kuyruk_.size();
    }

private:
    mutable std::mutex mtx_;
    std::queue<T> kuyruk_;
    std::condition_variable cv_;
    bool kapaniyor_ = false;
};

int main() {
    ThreadSafeQueue<std::string> kuyruk;

    // 2 tüketici
    std::vector<std::thread> tuketiciler;
    for (int id = 0; id < 2; ++id) {
        tuketiciler.emplace_back([&kuyruk, id] {
            std::string s;
            while (kuyruk.wait_and_pop(s)) {
                std::cout << "[T" << id << "] " << s << "\n";
            }
            std::cout << "[T" << id << "] kapanıyor\n";
        });
    }

    // 10 iş ekle
    for (int i = 0; i < 10; ++i) {
        kuyruk.push("Görev " + std::to_string(i));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "Kuyruktaki kalan: " << kuyruk.size() << "\n";

    kuyruk.close();
    for (auto& t : tuketiciler) t.join();

    std::cout << "Tamamlandı.\n";
    return 0;
}

/*
 * BEKLENEN ÇIKTI (sıra değişebilir):
 * ----------------------------------
 * [T0] Görev 0
 * [T1] Görev 1
 * ...
 * [T0] Görev 9
 * Kuyruktaki kalan: 0
 * [T0] kapanıyor
 * [T1] kapanıyor
 * Tamamlandı.
 */
