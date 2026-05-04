/**
 * Thread Pool ile Future Tabanlı Görev Gönderme
 *
 * Bu program, thread havuzuna farklı dönüş tipli görevler submit
 * edip her birinin sonucunu future ile alma örüntüsünü gösterir.
 * Ayrıca istisna aktarımı ve hata yönetimi vardır.
 *
 * Öğrenilecek kavramlar:
 * - Farklı tipli future<T> kullanımı
 * - Bir havuzda void ve değer döndüren görevlerin karışımı
 * - Havuz içinden istisna aktarımı
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 3 - Thread Havuzu Tasarımı
 *
 * Derleme: g++ -std=c++20 -pthread -o 03_pool_fut 03_thread_pool_future.cpp
 * Çalıştırma: ./03_pool_fut
 *
 * NOT: ThreadPool sınıfı 02_simple_thread_pool.cpp'deki ile aynıdır.
 */

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <iostream>
#include <string>
#include <chrono>
#include <stdexcept>

template <typename T>
class ThreadSafeQueue {
public:
    void push(T v) {
        { std::lock_guard<std::mutex> l(mtx_); if (kap_) return; q_.push(std::move(v)); }
        cv_.notify_one();
    }
    bool wait_and_pop(T& out) {
        std::unique_lock<std::mutex> l(mtx_);
        cv_.wait(l, [this] { return !q_.empty() || kap_; });
        if (q_.empty()) return false;
        out = std::move(q_.front()); q_.pop();
        return true;
    }
    void close() {
        { std::lock_guard<std::mutex> l(mtx_); kap_ = true; }
        cv_.notify_all();
    }
private:
    std::mutex mtx_;
    std::queue<T> q_;
    std::condition_variable cv_;
    bool kap_ = false;
};

class ThreadPool {
public:
    explicit ThreadPool(std::size_t n = std::thread::hardware_concurrency()) {
        if (n == 0) n = 1;
        for (std::size_t i = 0; i < n; ++i)
            isciler_.emplace_back([this] {
                std::function<void()> g;
                while (kuyruk_.wait_and_pop(g)) g();
            });
    }
    ~ThreadPool() {
        kuyruk_.close();
        for (auto& t : isciler_) if (t.joinable()) t.join();
    }
    template<typename F, typename... A>
    auto submit(F&& f, A&&... a)
        -> std::future<std::invoke_result_t<F, A...>>
    {
        using R = std::invoke_result_t<F, A...>;
        auto g = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<A>(a)...));
        auto fut = g->get_future();
        kuyruk_.push([g] { (*g)(); });
        return fut;
    }

private:
    std::vector<std::thread> isciler_;
    ThreadSafeQueue<std::function<void()>> kuyruk_;
};

// Örnek görevler
int karesi(int x) { return x * x; }
std::string selamla(const std::string& isim) {
    return "Merhaba " + isim;
}
double riskliBol(double a, double b) {
    if (b == 0) throw std::invalid_argument("Sıfıra bölme!");
    return a / b;
}

int main() {
    ThreadPool havuz(4);

    // int dönen görev
    auto f1 = havuz.submit(karesi, 7);
    std::cout << "7^2 = " << f1.get() << "\n";

    // string dönen görev
    auto f2 = havuz.submit(selamla, "Dünya");
    std::cout << f2.get() << "\n";

    // Lambda + yakalama
    int temel = 100;
    auto f3 = havuz.submit([temel](int ekle) {
        return temel + ekle;
    }, 42);
    std::cout << "100 + 42 = " << f3.get() << "\n";

    // İstisna aktarımı
    auto f4 = havuz.submit(riskliBol, 10.0, 0.0);
    try {
        double r = f4.get();
        std::cout << "bölüm = " << r << "\n";
    } catch (const std::exception& e) {
        std::cout << "Yakalandı: " << e.what() << "\n";
    }

    // void görev
    auto f5 = havuz.submit([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "void görev tamamlandı\n";
    });
    f5.wait();

    return 0;
}

/*
 * BEKLENEN ÇIKTI:
 * ---------------
 * 7^2 = 49
 * Merhaba Dünya
 * 100 + 42 = 142
 * Yakalandı: Sıfıra bölme!
 * void görev tamamlandı
 */
