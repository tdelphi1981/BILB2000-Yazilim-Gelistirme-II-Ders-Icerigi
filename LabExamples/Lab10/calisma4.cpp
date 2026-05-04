/**
 * Lab 10 - Çalışma 4: OBYS Toplu GPA (Thread Pool)
 *
 * Bu kod Çalışma 3'teki ThreadPool'u tamamladıktan sonra çalışır.
 * TODO yorumlarını doldurun.
 *
 * Derleme: g++ -std=c++20 -O2 -pthread -o calisma4 calisma4.cpp
 */

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <string>
#include <map>
#include <iostream>
#include <chrono>
#include <random>

// --- ThreadPool (Calisma 3'ten kopyalayin) ---
class ThreadPool {
public:
    explicit ThreadPool(std::size_t n) {
        if (n == 0) n = 1;
        for (std::size_t i = 0; i < n; ++i)
            isciler_.emplace_back([this] {
                while (true) {
                    std::function<void()> gorev;
                    {
                        std::unique_lock<std::mutex> kilit(mtx_);
                        cv_.wait(kilit, [this] {
                            return !kuyruk_.empty() || kapaniyor_;
                        });
                        if (kuyruk_.empty() && kapaniyor_) return;
                        gorev = std::move(kuyruk_.front());
                        kuyruk_.pop();
                    }
                    gorev();
                }
            });
    }
    ~ThreadPool() {
        { std::lock_guard<std::mutex> l(mtx_); kapaniyor_ = true; }
        cv_.notify_all();
        for (auto& t : isciler_) if (t.joinable()) t.join();
    }
    template<typename F, typename... A>
    auto submit(F&& f, A&&... a)
        -> std::future<std::invoke_result_t<F, A...>> {
        using R = std::invoke_result_t<F, A...>;
        auto g = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<A>(a)...));
        auto fut = g->get_future();
        { std::lock_guard<std::mutex> l(mtx_);
          kuyruk_.push([g] { (*g)(); }); }
        cv_.notify_one();
        return fut;
    }
private:
    std::vector<std::thread> isciler_;
    std::queue<std::function<void()>> kuyruk_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool kapaniyor_ = false;
};

// --- OBYS veri modeli ---
struct DersNotu { int kredi; std::string harf; };
struct Ogrenci {
    int numara;
    std::string ad;
    std::vector<DersNotu> notlar;
};
struct GPASonuc { int numara; double gpa; };

double harfPuan(const std::string& h) {
    static const std::map<std::string, double> t = {
        {"AA",4.0},{"BA",3.5},{"BB",3.0},{"CB",2.5},
        {"CC",2.0},{"DC",1.5},{"DD",1.0},{"FF",0.0}
    };
    auto it = t.find(h);
    return it != t.end() ? it->second : 0.0;
}

GPASonuc hesaplaGPA(const Ogrenci& o) {
    double k = 0, p = 0;
    for (const auto& n : o.notlar) {
        k += n.kredi;
        p += n.kredi * harfPuan(n.harf);
    }
    // Simule is yuku
    volatile int s = 0;
    for (int i = 0; i < 20000; ++i) s += i;
    return {o.numara, k > 0 ? p / k : 0.0};
}

std::vector<Ogrenci> testVerisi(int n) {
    std::vector<Ogrenci> v; v.reserve(n);
    std::mt19937 rng(42);
    std::vector<std::string> harfler = {"AA","BA","BB","CB","CC","DC"};
    for (int i = 0; i < n; ++i) {
        Ogrenci o;
        o.numara = 100000 + i;
        o.ad = "Ogrenci_" + std::to_string(i);
        int ds = 20 + static_cast<int>(rng() % 15);
        for (int d = 0; d < ds; ++d) {
            DersNotu dn; dn.kredi = 3;
            dn.harf = harfler[rng() % harfler.size()];
            o.notlar.push_back(dn);
        }
        v.push_back(std::move(o));
    }
    return v;
}

int main() {
    auto ogrenciler = testVerisi(5000);
    std::cout << ogrenciler.size() << " ogrenci uretildi\n\n";

    // TODO (Gorev 4.1): Seri versiyon + zaman olcum
    //   auto t0 = chrono::steady_clock::now();
    //   std::vector<GPASonuc> seri;
    //   for (const auto& o : ogrenciler)
    //       seri.push_back(hesaplaGPA(o));
    //   auto t1 = ...; olc ve yazdir

    // TODO (Gorev 4.2): Havuz + her ogrenci icin submit
    //   ThreadPool havuz(8);
    //   std::vector<std::future<GPASonuc>> futurelar;
    //   for (const auto& o : ogrenciler)
    //       futurelar.push_back(havuz.submit(hesaplaGPA, std::cref(o)));
    //   std::vector<GPASonuc> par1;
    //   for (auto& f : futurelar) par1.push_back(f.get());
    //   olc ve yazdir

    // TODO (Gorev 4.3): Havuz + 100'lu batch
    //   ThreadPool havuz2(8);
    //   std::vector<std::future<std::vector<GPASonuc>>> bFuturelar;
    //   for i += 100:
    //     son = min(i+100, size)
    //     bFuturelar.push_back(havuz2.submit([&, i, son] {
    //       std::vector<GPASonuc> yerel;
    //       for (auto k = i; k < son; ++k)
    //         yerel.push_back(hesaplaGPA(ogrenciler[k]));
    //       return yerel; }));
    //   // sonuclari birlestir ve olc

    return 0;
}
