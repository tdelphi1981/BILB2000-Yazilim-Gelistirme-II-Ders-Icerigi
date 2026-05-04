/**
 * Basit Thread Pool Uygulaması
 *
 * Bu program, herhangi bir callable'ı kabul eden ve future
 * döndüren bir thread havuzunun tam uygulamasını sunar.
 *
 * Öğrenilecek kavramlar:
 * - Thread havuzu mimarisi
 * - std::packaged_task ile görev paketleme
 * - std::shared_ptr ile taşınamaz nesneleri std::function ile sarma
 * - Graceful shutdown ile destructor
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 3 - Thread Havuzu Tasarımı
 *
 * Derleme: g++ -std=c++20 -pthread -o 02_pool 02_simple_thread_pool.cpp
 * Çalıştırma: ./02_pool
 */

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <iostream>
#include <chrono>

template <typename T>
class ThreadSafeQueue {
public:
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

    void close() {
        {
            std::lock_guard<std::mutex> kilit(mtx_);
            kapaniyor_ = true;
        }
        cv_.notify_all();
    }

private:
    std::mutex mtx_;
    std::queue<T> kuyruk_;
    std::condition_variable cv_;
    bool kapaniyor_ = false;
};

class ThreadPool {
public:
    explicit ThreadPool(
        std::size_t isci_sayisi = std::thread::hardware_concurrency())
    {
        if (isci_sayisi == 0) isci_sayisi = 1;
        isciler_.reserve(isci_sayisi);
        for (std::size_t i = 0; i < isci_sayisi; ++i) {
            isciler_.emplace_back(&ThreadPool::isci_dongusu, this);
        }
    }

    ~ThreadPool() {
        kuyruk_.close();
        for (auto& t : isciler_) {
            if (t.joinable()) t.join();
        }
    }

    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using R = std::invoke_result_t<F, Args...>;
        auto gorev = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<R> gelecek = gorev->get_future();
        kuyruk_.push([gorev]() { (*gorev)(); });
        return gelecek;
    }

    std::size_t isci_sayisi() const noexcept {
        return isciler_.size();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    void isci_dongusu() {
        std::function<void()> gorev;
        while (kuyruk_.wait_and_pop(gorev)) {
            gorev();
        }
    }

    std::vector<std::thread> isciler_;
    ThreadSafeQueue<std::function<void()>> kuyruk_;
};

int main() {
    ThreadPool havuz(4);
    std::cout << "Havuz " << havuz.isci_sayisi() << " işçi ile açıldı\n";

    // 8 iş submit et
    std::vector<std::future<int>> sonuclar;
    for (int i = 0; i < 8; ++i) {
        sonuclar.push_back(havuz.submit([i] {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return i * i;
        }));
    }

    // Tüm sonuçları topla
    long long toplam = 0;
    for (auto& f : sonuclar) {
        toplam += f.get();
    }
    std::cout << "Kare toplamları: " << toplam << "\n";
    // Beklenen: 0^2 + 1^2 + ... + 7^2 = 140
    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * Havuz 4 işçi ile açıldı
 * Kare toplamları: 140
 *
 * Çalışma süresi yaklaşık 400 ms (8*200/4) olmalı.
 */
