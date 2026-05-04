/**
 * OBYS Toplu Not Hesaplama (Thread Pool ile Paralel)
 *
 * Bu program, binlerce öğrencinin GPA'sını thread havuzu kullanarak
 * paralel hesaplar. Seri ve paralel sürelerini karşılaştırır.
 *
 * Öğrenilecek kavramlar:
 * - Gerçek ölçekli paralel hesaplama
 * - Amdahl yasası ve hızlanma ölçümü
 * - Batch (paketleme) optimizasyonu
 * - std::chrono ile süre ölçümü
 *
 * Bölüm: 10 - İleri Çok Kanallı Programlama
 * Ünite: 3 - Thread Havuzu Tasarımı
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o 04_toplu 04_obys_toplu_not.cpp
 * Çalıştırma: ./04_toplu
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
#include <map>
#include <chrono>
#include <random>

// Minimal ThreadPool (kısaltılmış versiyon)
template <typename T>
class TSQ {
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
    auto submit(F&& f, A&&... a) -> std::future<std::invoke_result_t<F, A...>> {
        using R = std::invoke_result_t<F, A...>;
        auto g = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<A>(a)...));
        auto fut = g->get_future();
        kuyruk_.push([g] { (*g)(); });
        return fut;
    }
private:
    std::vector<std::thread> isciler_;
    TSQ<std::function<void()>> kuyruk_;
};

// Veri modeli
struct DersNotu { int kredi; std::string harf; };
struct Ogrenci {
    int numara;
    std::string ad;
    std::vector<DersNotu> notlar;
};
struct GPASonuc { int numara; double gpa; };

static double harfToPuan(const std::string& h) {
    static const std::map<std::string, double> t = {
        {"AA",4.0},{"BA",3.5},{"BB",3.0},{"CB",2.5},
        {"CC",2.0},{"DC",1.5},{"DD",1.0},{"FF",0.0}
    };
    auto it = t.find(h);
    return it != t.end() ? it->second : 0.0;
}

GPASonuc bireyselHesapla(const Ogrenci& o) {
    double k = 0, p = 0;
    for (const auto& n : o.notlar) {
        k += n.kredi;
        p += n.kredi * harfToPuan(n.harf);
    }
    // Hesaplama iş yükünü simüle etmek için ufak bir bekleme
    volatile int sum = 0;
    for (int i = 0; i < 20000; ++i) sum += i;
    return {o.numara, k > 0 ? p / k : 0.0};
}

std::vector<Ogrenci> testVerisi(int n) {
    std::vector<Ogrenci> v; v.reserve(n);
    std::mt19937 rng(42);
    std::vector<std::string> harfler = {"AA","BA","BB","CB","CC","DC","DD"};
    for (int i = 0; i < n; ++i) {
        Ogrenci o;
        o.numara = 100000 + i;
        o.ad = "Ogrenci_" + std::to_string(i);
        int ds = 20 + static_cast<int>(rng() % 15);
        for (int d = 0; d < ds; ++d) {
            DersNotu n;
            n.kredi = 3;
            n.harf = harfler[rng() % harfler.size()];
            o.notlar.push_back(n);
        }
        v.push_back(std::move(o));
    }
    return v;
}

int main() {
    auto ogrenciler = testVerisi(5000);
    std::cout << ogrenciler.size() << " öğrenci oluşturuldu.\n\n";

    // Seri hesaplama
    {
        auto t0 = std::chrono::steady_clock::now();
        std::vector<GPASonuc> sonuclar;
        sonuclar.reserve(ogrenciler.size());
        for (const auto& o : ogrenciler) {
            sonuclar.push_back(bireyselHesapla(o));
        }
        auto t1 = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << "Seri (1 izlek): " << ms << " ms\n";
    }

    // Paralel hesaplama (farklı işçi sayıları)
    for (std::size_t n : {2u, 4u, 8u}) {
        ThreadPool havuz(n);
        auto t0 = std::chrono::steady_clock::now();

        std::vector<std::future<GPASonuc>> futurelar;
        futurelar.reserve(ogrenciler.size());
        for (const auto& o : ogrenciler) {
            futurelar.push_back(havuz.submit(bireyselHesapla, std::cref(o)));
        }

        std::vector<GPASonuc> sonuclar;
        sonuclar.reserve(ogrenciler.size());
        for (auto& f : futurelar) sonuclar.push_back(f.get());

        auto t1 = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << "Paralel (" << n << " işçi): " << ms << " ms\n";
    }
    return 0;
}

/*
 * BEKLENEN ÇIKTI (donanıma bağlı):
 * --------------------------------
 * 5000 öğrenci oluşturuldu.
 *
 * Seri (1 izlek): 2500 ms
 * Paralel (2 işçi): 1300 ms
 * Paralel (4 işçi): 680 ms
 * Paralel (8 işçi): 370 ms
 *
 * NOT: Süreler donanıma ve derleyici optimizasyonuna (-O2) bağlıdır.
 */
